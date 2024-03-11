BEGIN{
	print "输出开始"
}
{
	print $0
}
END{
	print "输出结束"
}
