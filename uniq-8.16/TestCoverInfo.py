#!/bin/bash -x

PROGRAM=$1
CURRDIR=$(pwd)
COV=$2			  #change to your own directory
merge=${CURRDIR}/path_generator/merge_bin_gcov_files.sh
getbin=${CURRDIR}/path_generator/get_bin_gcov_file.sh
INDIR=${CURRDIR}/testscript/kn
INDIR_CP=${CURRDIR}/inputfile
OUTDIR=${CURRDIR}/output.origin/kn

#Copy input files (in case tests might change them)
if [ -d $INDIR ]; then
    cp -r $INDIR ${INDIR_CP}
    chmod 755 -R ${INDIR_CP}
fi

#Clean
if [ ! -d $OUTDIR ]; then
    mkdir -p $OUTDIR
else
    rm -fr $OUTDIR/*
fi

#Use a tmp directory for execution
if [ ! -d tmp ]; then
    mkdir tmp
else
    chmod 755 -R tmp
    rm -fr tmp/*
fi

cd tmp

for dir in lcov real.gcov bin.gcov
do
        if [ ! -d $dir  ]; then
            mkdir $dir
        else
                chmod 755 -R $dir
                rm -fr $dir/*
        fi
done
cp ${CURRDIR}/path_generator/line.txt ${CURRDIR}/tmp/line.txt
cp $CURRDIR/$PROGRAM.c.real.origin.c $PROGRAM.c
cp $CURRDIR/$PROGRAM.c.real.origin.c $PROGRAM.c.real.origin.c

cp $CURRDIR/answer.txt $CURRDIR/tmp/answer.txt
cp $CURRDIR/data.txt $CURRDIR/tmp/data.txt
cp $CURRDIR/input $CURRDIR/tmp/input
cp $CURRDIR/input.origin/* $CURRDIR/tmp -r
clang -fprofile-instr-generate -fcoverage-mapping -w -o $PROGRAM $PROGRAM.c

#Execute every test
for testf in ${INDIR_CP}/*
do
    test_name=${testf##*/} 
    timeout 1s ${testf} $CURRDIR/tmp/$PROGRAM ${OUTDIR} 1 ${CURRDIR}/tmp
#mv default.profraw $CURRDIR/default.profraw
    $CURRDIR/path_generator/get_bin_gcov_file.sh $PROGRAM $COV	
    mv $PROGRAM.lcov $CURRDIR/tmp/lcov/${test_name}.lcov
    mv $PROGRAM.real.gcov $CURRDIR/tmp/real.gcov/${test_name}.real.gcov
    mv $PROGRAM.bin.gcov $CURRDIR/tmp/bin.gcov/${test_name}.bin.gcov
#mv $CURRDIR/default.profraw de.profraw
done

#Remove copied files
if [ -d ${INDIR_CP} ]; then
    chmod 755 -R ${INDIR_CP} #In case permissions are changed
    rm -fr ${INDIR_CP}
fi

$CURRDIR/path_generator/merge_bin_gcov_files.sh $COV

${COV}/bin/gcovbasedcoderemover $PROGRAM.c line.txt merged.bin.gcov false > ${PROGRAM}.c.cov.origin.c
mv ${PROGRAM}.c.cov.origin.c ../${PROGRAM}.c.cov.origin.c
