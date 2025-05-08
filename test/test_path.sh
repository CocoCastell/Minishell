#!/bin/bash

run_test() {
    local description="$1"
    local path_value="$2"
    local outputfile_minishell=$(mktemp)
    local outputfile_bash=$(mktemp)

    echo -e "\n\033[1;34m#################################\033[0m"
    echo -e "\033[1;34mTEST:\033[0m $description"

    # Run in minishell
    echo -e "export PATH=$path_value\nmycmd\necho \$?" | ../minishell > "$outputfile_minishell" 2>&1

    # Run in bash
    echo -e "export PATH=$path_value\nmycmd\necho \$?" | bash > "$outputfile_bash" 2>&1

    ## Cleanup minishell output
    if tail -n 1 "$outputfile_minishell" | grep -qE '^[^ ]* > *$'; then
        sed '$d' "$outputfile_minishell"
    else
        cat "$outputfile_minishell"
    fi

    # --- Extract output (command output and status)
    local minishell_output=$(grep -v '^$' "$outputfile_minishell" | grep -v '^\([^ ]*\) >' | head -n 1)
    local minishell_status=$(grep -v '^$' "$outputfile_minishell" | tail -n 2 | head -n 1)
    minishell_status="${minishell_status#*> }"

    local bash_output=$(head -n 1 "$outputfile_bash")
    local bash_status=$(tail -n 1 "$outputfile_bash")

    # --- Compare
    echo -e "\n\033[1;36mMINISHELL OUTPUT:\033[0m $minishell_output"
    echo -e "\033[1;36mBASH OUTPUT:\033[0m      $bash_output"

    echo -e "\033[1;36mMINISHELL STATUS:\033[0m $minishell_status"
    echo -e "\033[1;36mBASH STATUS:\033[0m      $bash_status"

    # --- Check matches
    if [[ "$minishell_output" == "$bash_output" && "$minishell_status" == "$bash_status" ]]; then
        echo -e "\n\033[1;32m✅ RESULT: MATCHES BASH\033[0m"
    else
        echo -e "\n\033[1;31m❌ RESULT: DOES NOT MATCH BASH\033[0m"
    fi

    rm "$outputfile_minishell" "$outputfile_bash"
}

# --- Prepare temp dirs
dir1=$(mktemp -d)
dir2=$(mktemp -d)

# Executable in dir1
echo -e '#!/bin/bash\necho "Ejecutado desde DIR1"' > "$dir1/mycmd"
chmod +x "$dir1/mycmd"

# Executable in dir2
echo -e '#!/bin/bash\necho "Ejecutado desde DIR2"' > "$dir2/mycmd"
chmod +x "$dir2/mycmd"

# --- Run tests
run_test "PATH: dir1 primero (debe ejecutar DIR1)" "$dir1:$dir2"
run_test "PATH: dir2 primero (debe ejecutar DIR2)" "$dir2:$dir1"

# Cleanup
rm -rf "$dir1" "$dir2"
