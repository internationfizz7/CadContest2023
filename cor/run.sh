
# Version 2020/02/05
# Author m107jjchen


BINPATH="./bin/MacroPlacement"
DEFLEF_FILE="/ift1/home/Himax/Bench"
PL_FILE="/ift1/home/Himax/GP/NCKUPL"
#PL_FILE="/ift1/home/Himax/GP/NCKUPL/SZU"
#PL_FILE="/ift1/home/Himax/GP/NCKUPL/yj/20201014_for_himax_gp/without_modify_CG"
#PL_FILE="/ift1/home/Himax/GP/NCKUPL/Y-T/dataflow"

#PL_FILE="/ift1/home/Himax/GP/NCKUPL/WF/normal"  # m108wfh's gp for case5.6.7.8.11 Datapath cases
#PL_FILE="/ift/home/m108yjhuang/Desktop/GP_v2"
#PL_FILE="/ift1/home/Himax/Week27_PL" #5.6.7.8.11 Datapath cases
#PL_FILE="/ift/home/m108pclu/Desktop/2020.10_TO_HIMAX_PL_with_modify_CG"
#CLUSTER_FILE="/ift1/home/Himax/GP/GP_cluster_result"
#CLUSTER_FILE="/ift/home/m108pclu/Desktop/2020.10_TO_HIMAX_PL_with_modify_CG"

PARA_FILE="./input/PARAMETER/SET_PARA"
DATAFLOW_FILE="/ift1/home/Himax/dataflow"
LOG_FILE="./output/Log"

rm ./output/*/*
#make clean
rm $BINPATH
make

echo ''
echo '[Now Run Macro Placement]'
case $1 in
	case1)
for((mod= 1; mod<= 3 ; mod++))
do
		echo "[choose PA5590]"
(time $BINPATH -lefdef ./bench/PA5590/PA5590 -para $PARA_FILE -loadpl ./bench/PA5590/PA5590_ans.pl -datapath ${filepath}/${filename}.datapath -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5590_MP_mod${mod}.log 
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done
	;;
	case2)
for((mod= 1; mod<= 3 ; mod++))
do
		echo "[choose TEST]"
(time $BINPATH -lefdef ./bench/case03/top/case03_top -para $PARA_FILE -loadpl ./bench/case03/top/case03_top.pl -datapath ${filepath}/${filename}.datapath -mod $mod -Refine 0 -debug 1) | tee $LOG_FILE/TEST_MP_mod.log
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done	
	;;
	case3)
for((mod= 1; mod<= 3 ; mod++))
do
		echo "[choose PA5651]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA5651 -para $PARA_FILE -loadpl $PL_FILE/PA5651_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5651_MP_mod.log
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done
	;;
	case4)
for((mod= 1; mod<= 3 ; mod++))
do
		echo "[choose PA5654]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA5654 -para $PARA_FILE -loadpl $PL_FILE/PA5654_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5654_MP_mod.log
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done
	;;
	case5)
for((mod= 1; mod<= 3 ; mod++))
do
		echo "[choose PA5656B]"

(time $BINPATH -lefdef $DEFLEF_FILE/PA5656B -para $PARA_FILE -loadpl $PL_FILE/PA5656B_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5656B_MP_mod.log
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done
	;;
	case6)
for((mod= 1; mod<= 3 ; mod++))
do
		echo "[choose PA5617]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA5617 -para $PARA_FILE -loadpl $PL_FILE/PA5617_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5617_MP_mod.log
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done
	;;
	case7)
for((mod= 3; mod<= 3 ; mod++))
do
		echo "[choose PA5658A_TX]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA5658A_TX -para $PARA_FILE -loadpl $PL_FILE/PA5658A_TX_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5658A_MP_mod.log
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done
	;;
	case8)
for((mod= 1; mod<= 3 ; mod++))
do
		echo "[choose PA5658B]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA5658B -para $PARA_FILE -loadpl $PL_FILE/PA5658B_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5658B_MP_mod.log
rename ans mod${mod}_ans ./output/Result/ans*
rename Region mod${mod}_Region ./output/subregion_graph/Region*
done
	;;
	case9)
for((mod= 1; mod<= 1 ; mod++))
do
		echo "[choose PA5663]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA5663 -para $PARA_FILE -loadpl $PL_FILE/PA5663_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5663_MP.log
rename ans mod${mod}_ans ./output/Result/ans*
done
	;;
	case10)
for((mod= 1; mod<= 1 ; mod++))
do
		echo "[choose PA5668A]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA5668A -para $PARA_FILE -loadpl $PL_FILE/PA5668A_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA5668A_MP.log
rename ans mod${mod}_ans ./output/Result/ans*
done
	;;
	case11)
for((mod= 1; mod<= 1 ; mod++))
do
		echo "[choose PA8536A]"
(time $BINPATH -lefdef $DEFLEF_FILE/PA8536A -para $PARA_FILE -loadpl $PL_FILE/PA8536A_ans.pl -mod $mod -Refine 1 -debug 1) | tee $LOG_FILE/PA8536A_MP.log
rename ans mod${mod}_ans ./output/Result/ans*
done
	;;

esac
echo ''

# PA5658A_TX SUZ cell distribution is not well, and Y-J result cell cluster isn't spare done

##################### [for "gdb" debuger comment] #####################

# gdb --args ./bin/MacroPlacement -case 1 -lefdef /ift1/home/Himax/Bench/PA5590 -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA5590_ans.pl -CongestionMap 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 2 -lefdef /ift1/home/Himax/Bench/PA5645 -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA5645_ans.pl -CongestionMap 1 -Refine 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 3 -lefdef /ift1/home/Himax/Bench/PA5651 -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA5651_ans.pl -CongestionMap 1 -Refine 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 4 -lefdef /ift1/home/Himax/Bench/PA5651 -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA5651_ans.pl -CongestionMap 1 -Refine 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 6 -lefdef /ift1/home/Himax/Bench/PA5617 -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA5617_ans.pl -CongestionMap 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 7 -lefdef /ift1/home/Himax/Bench/PA5658A_TX -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA5658A_TX_ans.pl -CongestionMap 1 -Refine 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 8 -lefdef /ift1/home/Himax/Bench/PA5658B -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA5658B_ans.pl -CongestionMap 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 9 -lefdef /ift1/home/Himax/Bench/PA5663 -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/yj/PA5663_ans.pl  -CongestionMap 1 -debug 1

# gdb --args ./bin/MacroPlacement -case 10 -lefdef /ift1/home/Himax/Bench/PA8536A -para ./input/PARAMETER/SET_PARA -loadpl /ift1/home/Himax/GP/NCKUPL/SZU/PA8536A_ans.pl  -CongestionMap 1 -debug 1

