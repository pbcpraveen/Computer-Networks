set ns [new Simulator]

$ns color 1 Blue
$ns color 2 Red

set nf [open out1.nam w]
$ns namtrace-all $nf

proc finish {} {
	global ns nf
	$ns flush-trace
	close $nf
	exec nam out.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]
set n12 [$ns node]
set n13 [$ns node]
set n14 [$ns node]

$ns duplex-link $n0 $n1 1.5Mb 50ms DropTail
$ns duplex-link $n0 $n2 1.5Mb 50ms DropTail
$ns duplex-link $n0 $n3 1.5Mb 50ms DropTail
$ns duplex-link $n0 $n9 1.5Mb 50ms DropTail
$ns duplex-link $n1 $n2 1.5Mb 50ms DropTail
$ns duplex-link $n1 $n7 1.5Mb 50ms DropTail
$ns duplex-link $n2 $n6 1.5Mb 50ms DropTail
$ns duplex-link $n3 $n4 1.5Mb 50ms DropTail
$ns duplex-link $n3 $n5 1.5Mb 50ms DropTail
$ns duplex-link $n3 $n6 1.5Mb 50ms DropTail
$ns duplex-link $n4 $n13 1.5Mb 50ms DropTail
$ns duplex-link $n5 $n13 1.5Mb 50ms DropTail
$ns duplex-link $n5 $n11 1.5Mb 50ms DropTail
$ns duplex-link $n7 $n8 1.5Mb 50ms DropTail
$ns duplex-link $n7 $n12 1.5Mb 50ms DropTail
$ns duplex-link $n8 $n9 1.5Mb 50ms DropTail
$ns duplex-link $n8 $n10 1.5Mb 50ms DropTail
$ns duplex-link $n9 $n11 1.5Mb 50ms DropTail

$ns queue-limit $n7 $n12 5
$ns queue-limit $n0 $n3 10
$ns queue-limit $n2 $n6 5
$ns queue-limit $n9 $n8 3
$ns queue-limit $n5 $n13 5


$ns duplex-link-op $n0 $n1 orient up
$ns duplex-link-op $n0 $n2 orient right-up
$ns duplex-link-op $n0 $n3 orient right
$ns duplex-link-op $n0 $n9 orient down
$ns duplex-link-op $n1 $n2 orient right
$ns duplex-link-op $n1 $n7 orient left-down
$ns duplex-link-op $n2 $n6 orient right
$ns duplex-link-op $n3 $n4 orient right
$ns duplex-link-op $n3 $n5 orient down
$ns duplex-link-op $n3 $n6 orient right-up
$ns duplex-link-op $n4 $n13 orient down
$ns duplex-link-op $n5 $n13 orient right
$ns duplex-link-op $n5 $n11 orient right-down
$ns duplex-link-op $n7 $n8 orient down
$ns duplex-link-op $n7 $n12 orient right
$ns duplex-link-op $n8 $n9 orient left
$ns duplex-link-op $n9 $n11 orient left-down
$ns duplex-link-op $n8 $n10 orient right


$ns duplex-link-op $n7 $n12 queuePos 5
$ns duplex-link-op $n0 $n3 queuePos 10
$ns duplex-link-op $n2 $n6 queuePos 5
$ns duplex-link-op $n9 $n8 queuePos 3
$ns duplex-link-op $n5 $n13 queuePos 5

set tcp [new Agent/TCP/Newreno]
$tcp set class_ 2
$ns attach-agent $n1 $tcp
set sink [new Agent/TCPSink/DelAck]
$ns attach-agent $n11 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
$tcp set window_ 8000
$tcp set packetSize_ 512
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP

set udp [new Agent/UDP]
$ns attach-agent $n4 $udp
set null [new Agent/Null]
$ns attach-agent $n12 $null
$ns connect $udp $null
$udp set fid_ 2
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set type_ CBR
$cbr set packet_size_ 512
$cbr set rate_ 0.01mb
$cbr set random_ false


$ns at 0.1 "$cbr start"
$ns at 0.5 "$ftp start"
$ns at 8.0 "$ftp stop"
$ns at 8.5 "$cbr stop"

$ns at 8.5 "$ns detach-agent $n1 $tcp ; $n11 detach-agent $n5 $sink"
$ns at 9.0 "finish"

puts "CBR packet size = [$cbr set packet_size_]"
puts "CBR interval = [$cbr set interval_]"

$ns run






