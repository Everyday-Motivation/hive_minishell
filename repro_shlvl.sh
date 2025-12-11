unset SHLVL
./minishell <<EOF
echo "Inside minishell"
env | grep SHLVL
EOF
php_exit=$?
if [ $php_exit -ne 0 ]; then
    echo "Minishell failed to start or exited with error"
fi
