M=4
N=4
data="$M $N"
for i in $(seq 1 $M)
do
    for j in $(seq 1 $N)
    do
        s=$((3*i*j*i/4+2*j*i*j/6+i*3-2*j+2+1))
        data="$data $s"
    done 
done


echo $data >> data.txt