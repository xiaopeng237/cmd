#!/usr/bin/expect -f
if {$argc < 3} {
    puts "Usage:cmd <host> <username> <password>"
    exit 1
}
set ip [lindex $argv 0]
set user [lindex $argv 1]
set pass [lindex $argv 2]
set timeout 15
spawn scp ./cmd/bin/cmd $user@$ip:/tmp
expect {
 "*yes/no" {send "yes\r"; exp_continue}
 "*password:" {send "$pass\r"}
}
spawn scp ./iicCtrl/lib/libiicCtrl.so $user@$ip:/tmp
expect {
 "*yes/no" {send "yes\r"; exp_continue}
 "*password:" {send "$pass\r"}
}
spawn scp ./spiCtrl/lib/libspiCtrl.so $user@$ip:/tmp
expect {
 "*yes/no" {send "yes\r"; exp_continue}
 "*password:" {send "$pass\r"}
}
expect eof
