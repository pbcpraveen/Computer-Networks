set ns [new Simulator]
$ns color 1 Red
set nf [open out1.nam w]
set nt [open out1.tr w]
$ns namtrace-all $nf
$ns trace-all $nt


proc finish {} {
	global ns nf nt 
	$ns flush-trace 
	close $nf
	close $nt
	exec nam out1.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
$ns duplex-link $n0 $n1 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns simplex-link $n2 $n1 2Mb 10ms DropTail

$ns queue-limit $n1 $n2 5

$ns duplex-link-op $n0 $n1 orient right
$ns simplex-link-op $n1 $n2 orient right

set tcp [new Agent/TCP]
$ns attach-agent $n0 $tcp
$tcp set class_ 2
set sink [new Agent/TCPSink/DelAck]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
$tcp set packetSize_ 512
$tcp set window_ 8000

set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP


$ns at 0.5 "$ftp start"
$ns at 4.5 "$ftp stop"

$ns at 5.0 "finish"
$ns run

