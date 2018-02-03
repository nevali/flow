#! /bin/sh

src="$1"
out="`basename $1`"
case "$out" in
	"")
		echo "Usage: $0 NAME.flow" >&2
		exit 99
		;;
	*.flow)
		base=`echo $out | sed 's@\.flow$@@g'`
		;;
	*)
		base=$out
		;;
esac

output="${base}.output"
expected="${base}.expected"

echo "source is ${src}, output is ${output}, expected output is ${expected}" >&2

echo Test: $*... >&2
if [ "x`echo $* | grep args`" != "x" ] ; then
	../interpreter/picoc ${src} - arg1 arg2 arg3 arg4 >${output}
else
	../interpreter/picoc ${src} >${output}; \
fi

if [ "x`diff -qbu ${expected} ${output}`" != "x" ]; then \
	echo "error in test $*" >&2
	diff -u ${expected} ${output}
	exit 1
fi

exit 0
