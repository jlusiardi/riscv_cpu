function check_for_binary() {
    COMMAND=`command -v $1`
    if [ $? != 0 ]; then
        echo -e "\e[31mFailed to find >$1< in path ($PATH)!\e[39m"
        exit 1
    else
        echo -e "\e[92mUsing $COMMAND for $1.\e[0m"
    fi
}

function set_red_foreground() {
    echo -e "\e[31m"
}

function set_magenta_foreground() {
    echo -e "\e[35m"
}

function set_green_foreground() {
    echo -e "\e[92m"
}

function set_normal_foreground() {
    echo -e "\e[0m"
}

function show_file() {
    set_magenta_foreground
    cat $1
    set_normal_foreground
}