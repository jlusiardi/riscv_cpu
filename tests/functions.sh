function check_for_binary() {
    COMMAND=`command -v $1`
    if [ $? != 0 ]; then
        echo -e "\e[31mFailed to find >$1< in path ($PATH)!\e[39m"
        exit 1
    else
        echo -e "\e[92mUsing $COMMAND for $1.\e[0m"
    fi
}
