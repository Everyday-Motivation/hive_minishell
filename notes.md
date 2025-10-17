# Notes

## Examples
echo hi > hi.txt | cat
ls -l | grep txt > out.txt
echo hello > out.txt

## Test cases
cd out of directory that has been deleted.
ctrl + c in random cases.
What if path is unset?
What if everything is unset?

### Quote
“           “
‘|’ {cmd}
""
```
cat <<EOF
line1
$HOME
EOF
```

cat <<'EOF'
line1
$HOME should NOT expand here
EOF

echo 'Home is $HOME'
echo "Home is $HOME"
echo "This is '$USER'"
echo 'This is "'$USER'"'


### Redirection
< {FileThatDoesntExist} echo hi | echo okii
< {FileThatDoesntExist} -> {FileThatDoesntExist}
echo "Hello '$USER'" > hello.txt
echo 'Hello "'$USER'"' >> hello.txt

### Pipes
echo "user:$USER" | tee tmp.txt | cat -n > numbered.txt

## Additional
Syntax error return value?
“.”/’m’”ini”s’h’’e’”ll” 

echo -e "hey" > in.txt
cat | cat | cat | cat > out.txt < in.txt


## Links
(Noel's Minishell Notes)[https://docs.google.com/document/d/1s4wMZveKFOlRsZxNinlzhRAKeUIjM6l2Ze_XBC2Ge2k/edit?tab=t.0]