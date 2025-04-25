#/bin/sh

while getopts w: flag
do
    case "${flag}" in
        w) wrapper=${OPTARG};;
    esac
done


if [ $wrapper == "python" ]; then
    cmake -S app -B build
    make -C build
    .venv/bin/python gui/triple_triad.py
    exit 1
elif [ $wrapper == "c" ]; then
    cmake -S app -B build
    make -C build
    ./build/TripleTriadCore
fi