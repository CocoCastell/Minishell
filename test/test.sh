#!/bin/bash

# Improved tester that works without expect

# Function to run a test and show exit status
run_test() {
    local command="$1"
    local outputfile_minishell
    local outputfile_bash
    outputfile_minishell=$(mktemp)
    outputfile_bash=$(mktemp)

    echo -e "\n\033[1;34m#################################\033[0m"
    echo -e "\033[1;34mTEST:\033[0m $command"
    echo -e "\n\033[1;34m#################################\033[0m"
    
    # Function to clean up prompt artifacts
    clean_prompt_artifacts() {
        local file=$1
        # Remove prompt lines and empty lines
        sed -i '/^[^ ]* >/d' "$file"
        sed -i '/^$/d' "$file"
    }

    # Run in minishell - using a here document to simulate input
    {
        echo "$command"
        echo "echo \$?"
    } | ../minishell > "$outputfile_minishell" 2>&1

    # Run in bash
    {
        echo "$command"
        echo "echo \$?"
    } | bash > "$outputfile_bash" 2>&1

    # Clean prompt artifacts from minishell output
    clean_prompt_artifacts "$outputfile_minishell"

    # Get outputs and statuses
    local minishell_output=$(head -n -1 "$outputfile_minishell" | tr -d '\r')
    local minishell_status=$(tail -n 1 "$outputfile_minishell" | tr -d '\r')

    local bash_output=$(head -n -1 "$outputfile_bash" | tr -d '\r')
    local bash_status=$(tail -n 1 "$outputfile_bash" | tr -d '\r')

    # Display results
    echo -e "\n\033[1;36mMINISHELL OUTPUT:\033[0m"
    echo "$minishell_output"
    echo -e "\033[1;36mMINISHELL STATUS:\033[0m $minishell_status"

    echo -e "\n\033[1;36mBASH OUTPUT:\033[0m"
    echo "$bash_output"
    echo -e "\033[1;36mBASH STATUS:\033[0m $bash_status"

    # Compare results
    if diff -q <(echo "$minishell_output") <(echo "$bash_output") >/dev/null && \
       [ "$minishell_status" = "$bash_status" ]; then
        echo -e "\n\033[1;32m✅ RESULT: MATCHES BASH\033[0m"
    else
        echo -e "\n\033[1;31m❌ RESULT: DOES NOT MATCH BASH\033[0m"
        echo -e "\n\033[1;33m#################################\033[0m"
        echo -e "\033[1;33mDifferences:\033[0m"
        diff -u <(echo "$bash_output") <(echo "$minishell_output") | sed 's/^/  /'
        echo -e "\n\033[1;33m#################################\033[0m" 
    fi

    rm "$outputfile_minishell" "$outputfile_bash"
}

# Function to test interactive features
test_interactive() {
    echo -e "\n\033[1;35mTESTING INTERACTIVE FEATURES\033[0m"
    
    # Test empty input
    run_test ""
    
    # Test whitespace input
    run_test "   "
    run_test $'\t'
    
    # Test simple commands
    run_test "ls"
    run_test "pwd"
    
    # Test error cases
    run_test "command_that_doesnt_exist"
    run_test "ls /nonexistent_directory"
    
    # Test builtins
    run_test "cd"
    run_test "echo hello"
    run_test "export TEST_VAR=123"
    run_test "unset TEST_VAR"
}

# ------------------------------
# Test Categories
# ------------------------------

test_general() {
    run_test '/bin/ls'
    run_test ''
    run_test '\t  '
    run_test '   '
    run_test '/bin/pwd aaa'
    run_test '/bin/cd /src'
    run_test '/bin/echo Hey'
}

test_echo() {
    run_test 'echo '
    run_test 'echo'
    run_test 'echo Hello World'
    run_test 'echo -n Hello World'
    run_test 'echo "Hello      World"'
    run_test "echo 'Hello      World'"
    run_test 'echo Hello" "World'
    run_test 'echo Hello""World'!'"!"'
    run_test 'echo Hello'\''World'
    run_test 'echo "Hello'\''World"'
    run_test 'echo $USER'
    run_test 'echo "$USER"'
    run_test 'echo \$USER'
    run_test 'echo ~'
    run_test 'echo "~"'
    run_test 'echo *'
    run_test 'echo "*"'
    run_test 'echo ""'
    run_test 'echo $NONEXISTENT_VAR'
    run_test 'echo "$NONEXISTENT_VAR"'
    run_test 'echo "" """" "" "" ""-n -nnnn a'
    run_test 'echo "" """" "" "" "" -n -nnnn a'
}

test_exit() {
    run_test '/bin/exit 34'
    run_test 'exit'
    run_test 'exit ""'
    run_test 'exit   '
    run_test 'exit 1'
    run_test 'exit 42'
    run_test 'exit 255'
    run_test 'exit 256'
    run_test 'exit -1'
    run_test 'exit too many arguments'
}

test_return() {
    run_test '/bin/ls filethatdoesntexist'
    run_test '$? + $?'
    run_test 'echo $? + $?'
}

test_quotes(){
    ## Test doble quotes ##
    run_test '"/bin/ls"'
    run_test '"echo hola && pwd"'
    run_test '"echo""USER"'
    run_test '"echo ""USER"'
    run_test '"echo" "USER"'
    run_test '"echo" "NONVAR"'
    run_test '"echo" "USERhola"'
    run_test 'echo "cat lol.c | cat > lol.c"'

    ## Test simple quotes ##
    run_test "'/bin/ls'"
    run_test "'echo hola && pwd'"
    run_test "'echoUSER'"
    run_test "'echo ''USER'"
    run_test "'echo' 'USER'"
    run_test "echo 'cat lol.c | cat > lol.c'"
    run_test "'echo \$USER'"
}

test_vars() {
    run_test '"echo""$USER"'
    run_test '"echo ""$USER"'
    run_test '"echo" "$USER"'
    run_test '"echo" "$NONVAR"'
    run_test '"echo" "$USERhola"'
    run_test 'echo $"42$"'
    run_test 'echo a$"42$"'
    run_test 'echo a$"""42$"'
    run_test 'echo a$""'
    run_test 'echo hola$"USER"'
    run_test 'echo a$a"USER$"a$'
    run_test 'echo a$"a$"'
    run_test 'echo $"$"'
    run_test 'echo "$USER$+"'
    run_test 'echo "$USER$"'
    run_test 'echo -asas$USER$?$%$$-$'
    run_test 'echo $"$USER$+$$$-$2323"$'
    run_test 'echo $"$USER$+$$$-$A2323"$'
    run_test 'echo as--as$USER$?$%$$-$&asasas&&ls'
    run_test 'echo $"$$$$$$$$"$"$$ $$ $ $$$"$$"a"a$"a"$a$"a"a$'
}

test_cd() {
    run_test 'cd'
    run_test 'cd && pwd'
    run_test 'cd ~'
    run_test 'cd ~ && pwd'
    run_test 'cd -'
    run_test 'cd - && pwd'
    run_test 'cd --'
    run_test 'cd -- && pwd'
    run_test 'cd .'
    run_test 'cd . && pwd'
    run_test 'cd ./'
    run_test 'cd ./ && pwd'
    run_test 'cd ..'
    run_test 'cd .. && pwd'
    run_test 'cd ../'
    run_test 'cd ../ && pwd'
    run_test 'cd /tmp'
    run_test 'cd /tmp && pwd'
    run_test 'cd tmp'
    run_test 'cd tmp && pwd'
    run_test 'cd /nonexistent'
    run_test 'cd /nonexistent && pwd'
    run_test 'cd too many arguments'
    run_test 'cd too many arguments && pwd'
    run_test 'pwd'
    run_test '/bin/pwd'
    run_test 'pwd extra argument'
}

test_export() {
    run_test 'export MY_VAR=hello'
    run_test 'env | grep MY_VAR'
    run_test 'export MY_VAR="hello world"'
    run_test 'export MY_VAR=hello=world'
    run_test 'export MY_VAR='
    run_test 'export MY_VAR'
    run_test 'export 1VAR=invalid'
    run_test 'export _VAR=valid'
    run_test 'export VAR+=append'
    run_test 'export VAR$=invalid'
    run_test 'export VAR1 VAR2=value VAR3'
    run_test 'export VAR1 VAR2=value VAR3+=asas A- A]=asas'
    run_test 'env | grep USER'
}

test_unset() {
    run_test 'unset MY_VAR'
    run_test 'unset NONEXISTENT_VAR'
    run_test 'ls && unset PATH && ls'  # Dangerous but should be tested
    run_test 'unset _VAR'
    run_test 'unset 1VAR'  # Invalid but should fail gracefully
    run_test 'unset VAR1 VAR2 VAR3'
    run_test 'env | grep MY_VAR'
}

test_env() {
    run_test 'env extra argument'
    run_test 'env'
    run_test 'export'
}
# ------------------------------
# Test runner
# ------------------------------
run_all_tests() {
    echo -e "\n\033[1;32mRUNNING ALL TESTS\033[0m\n"
    test_general
    test_echo
    test_exit
    test_return
    test_quotes
    test_vars
    test_cd
    test_export
    test_unset
    test_env
}

#!/bin/bash

# Function to clean up test files
cleanup() {
    rm -f out out1 out2 testfile heredocfile 2>/dev/null
}

# ------------------------------
# Pipe and Redirection Tests
# ------------------------------

test_pipes_redirs() {
    echo -e "\n\033[1;35mTESTING PIPES AND REDIRECTIONS\033[0m"

    # Basic pipes
    run_test "ls | wc -l"
    run_test "echo hello | cat | wc -c"
    run_test "ls | grep .c | wc -l"

    # Redirections
    run_test "echo hello > out && cat out"
    run_test "ls > out && wc -l < out"
    run_test "< Makefile wc -l > out && cat out"

    # Combined operations
    run_test "pwd && ls | wc -l"
    run_test "ls *.c > out1 >out2"
    run_test "ls *.c > out1 2 >out2"
    run_test "ls *.c > out1 2>out2"
    run_test "ls *.c > out1 2 >out2 || echo failed"
    run_test "ls *.c > out1 2>out2 || echo failed"

    # Multiple redirections
    run_test "echo test > out1 > out2 && cat out1 out2"
    run_test "cat < Makefile > out && head -n 3 out"

    # Error cases
    run_test "ls | non_existent_command"
    run_test "cat < non_existent_file"
    run_test "echo hello > /nonexistent/dir/out"
   
    # Complex combinations
    run_test "ls *.c | tee out1 | wc -l > out2 && cat out1 out2"
    run_test "echo start && (ls | wc -l) || echo failed"
    run_test '(echo hola || ls && pwd) && (cd asadad || pwd)'
    cleanup
}

# ------------------------------
# Heredoc Edge Cases
# ------------------------------

test_heredoc_edge_cases() {
    echo -e "\n\033[1;35mTESTING HEREDOC EDGE CASES\033[0m"

    # Empty heredoc
    run_test "cat << EOF\nEOF"

    # Heredoc with special chars
    run_test "cat << EOF\n\$USER\n\"quotes\"\n'singles'\nEOF"

    # Heredoc with tabs
    run_test "cat << EOF\n\tindented\nEOF"

    # Multiple heredocs
    run_test "cat << ONE && cat << TWO\nfirst\nONE\nsecond\nTWO"

    # Heredoc in pipes
    run_test "cat << END | cat\npipe content\nEND"

    # Heredoc with commands
    run_test "cat << EOF\necho \$USER\nEOF"

    # Heredoc delimiter with special chars
    run_test "cat << 'EOF$'\ncontent\nEOF$"
    run_test "cat << 'EOF!'\ncontent\nEOF!"

     # Heredocs
    run_test "cat << EOF\nhello world\nEOF"
    run_test "cat << 'EOF'\nhello \$USER\nEOF"
    run_test "wc -l << EOF\nline1\nline2\nEOF"

    # Combined pipes and heredocs
    run_test "cat << a | cat << 'a'\ncontent1\na\ncontent2\na"
    run_test "cat << a | cat << 'a' | ls\ndata1\na\ndata2\na"
    run_test "cat << a | cat << 'a' | ls | cat << \$USER\ndata1\na\ndata2\na"

    run_test "cat << a > out && grep hola out || ls\nhola mundo\na"
    run_test "cat << EOF > /nonexistent/dir/out\ncontent\nEOF"
    run_test "cat << EOF | grep hello | wc -l > out\nhello\nworld\nhello again\nEOF"

    cleanup
}

cleanup
# ------------------------------
# Main
# ------------------------------

case "$1" in
    all) 
        test_interactive
        test_echo
        test_exit
        test_pipes_redirs
        test_heredoc_edge_cases
        test_return
        test_cd
        test_general
        test_quotes
        test_vars
        test_export
        test_unset
        test_env
        ;;
    interactive) test_interactive ;;
    echo) test_echo ;;
    return) test_return ;;
    cd) test_cd ;;
    exit) test_exit ;;
    general) test_general ;;
    quotes) test_quotes ;;
    vars) test_vars ;;
    export) test_export ;;
    unset) test_unset ;;
    env) test_env ;;
    pipes) test_pipes_redirs ;;
    heredoc) test_heredoc_edge_cases ;;
    *)
        echo -e "\033[1;33mUsage:\033[0m $0 [all|interactive|echo|...]"
        ;;
esac
