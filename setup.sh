DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))
export LD_LIBRARY_PATH=$DIR_TOP/src/lib:$LD_LIBRARY_PATH

function rebuild-ana {
    CURRDIR=`pwd`
    cd $DIR_TOP/src
    mkdir -p build
    cd build
    make clean
    #rm -rf *
    cmake -DCMAKE_INSTALL_PREFIX=$DIR_TOP/src ..
    make install
    ret=$?
    test $ret -eq 0 && echo "Re-build done'."
    cd $CURRDIR
}

FN_LIST=list_reco.txt

function make-reco-list {
    echo "Command: $0 $*"
    echo "Time: $(date '+%F %H:%M:%S')"

    NUM_TOT=0
    SIZE_TOT=0

    for DIR in $* ; do
        DIR_FULL=$(readlink -f $DIR)
        echo "Directory = $DIR_FULL"
        while read NAME SIZE ; do
            printf "  %20s  %6d MB \n" $NAME $(( $SIZE/1024/1024 ))
            echo $DIR_FULL/$NAME >&3
            NUM_TOT=$(( $NUM_TOT + 1 ))
            SIZE_TOT=$(( $SIZE_TOT + $SIZE / 1024 ))
        done < <(find $DIR_FULL -name result.root -printf '%P %s\n')
    done 3>$FN_LIST

    echo
    echo "Total: $NUM_TOT files, $(( $SIZE_TOT/1024 )) MB"
}
