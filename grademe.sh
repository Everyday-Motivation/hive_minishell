#!/usr/bin/env bash

TEST_FILE="${1:-tests.txt}"
MINISHELL="${2:-./minishell}"

if [[ ! -f "$TEST_FILE" ]]; then
    echo "Test file '$TEST_FILE' not found."
    exit 1
fi

if [[ ! -x "$MINISHELL" ]]; then
    echo "Minishell executable '$MINISHELL' not found or not executable."
    exit 1
fi

BOLD="\033[1m"
GREY="\033[90m"
RESET="\033[0m"

normalize_output() {
    sed -E \
        -e 's/^(minishell|bash):[[:space:]]*//g' \
        -e 's/^line[[:space:]]+[0-9]+:[[:space:]]*//g' \
        -e 's/bash: line [0-9]+:[[:space:]]*//g' \
        -e 's/[[:space:]]+/ /g' \
        -e 's/^[[:space:]]+//; s/[[:space:]]+$//'
}

test_num=0
passed=0
similar=0
failed=0

while IFS= read -r line || [[ -n "$line" ]]; do
    if [[ -z "$line" || "${line:0:1}" == "#" ]]; then
        continue
    fi

    ((test_num++))

    mini_out="$(mktemp)"
    bash_out="$(mktemp)"

    printf '%s\n' "$line" | "$MINISHELL" >"$mini_out" 2>&1
    mini_status=$?

    printf '%s\n' "$line" | bash >"$bash_out" 2>&1
    bash_status=$?

    exact=false
    if diff -q "$bash_out" "$mini_out" >/dev/null 2>&1; then
        exact=true
    fi

    mini_norm="$(normalize_output < "$mini_out")"
    bash_norm="$(normalize_output < "$bash_out")"
    norm=false
    if [[ "$mini_norm" == "$bash_norm" ]]; then
        norm=true
    fi

    if $exact && [[ $mini_status -eq $bash_status ]]; then
        printf "${BOLD}Test %3d:${RESET} ✅ ${GREY}%s${RESET}\n" "$test_num" "$line"
        ((passed++))

    elif $norm && [[ $mini_status -eq $bash_status ]]; then
        printf "${BOLD}Test %3d:${RESET} ✅ (similar) ${GREY}%s${RESET}\n" "$test_num" "$line"
        ((similar++))

    else
        printf "${BOLD}Test %3d:${RESET} ❌ ${GREY}%s${RESET}\n" "$test_num" "$line"
        mini_text="$(cat "$mini_out")"
        bash_text="$(cat "$bash_out")"
        printf 'mini[%d]: %s\n' "$mini_status" "$mini_text"
        printf 'bash[%d]: %s\n' "$bash_status" "$bash_text"
        ((failed++))
    fi

    rm -f "$mini_out" "$bash_out"
done < "$TEST_FILE"

total_good=$((passed + similar))
echo
printf "${BOLD}%s/%s${RESET}\n" "$total_good" "$test_num"
