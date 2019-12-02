```
for V in `ls ../main/*.v`
do
    echo "set_global_assignment -name VERILOG_FILE $V -hdl_version SystemVerilog_2005"
done
```