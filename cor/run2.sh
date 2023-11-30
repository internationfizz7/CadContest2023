filepath="/ift1/home/Himax/Bench"
filename="PA5617"

rm ./output/*/*
rm ./output/*/*/*

echo 'Now run' ${filename}
### Placement Prototyping
(time ./bin/NCKUplacerV2 -parser 1 -benName ${filepath}/${filename} -plotName ${filename} -clusNum 6000 -datapath 0 -decluster 0 -decluStd 1000) | tee ./output/Log/${filename}_GP.log


### Macro Placement
for((mod= 1; mod<= 3 ; mod++))
do
(time ./bin/MacroPlacement -lefdef ${filepath}/${filename} -para ./input/PARAMETER/SET_PARA -loadpl ./${filename}_ans.pl -datapath ${filepath}/${filename}.datapath -mod $mod -Refine 1 -debug 1) | tee ./output/Log/${filename}_MP.log
rename ans mod${mod}_ans ./output/Result/ans*
done

