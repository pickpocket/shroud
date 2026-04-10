; ModuleID = 'argparse.c'
source_filename = "argparse.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.44.35224"

%struct.ParseResult = type { [16 x %struct.OptValue], i32, [32 x [256 x i8]], i32, i32, [256 x i8] }
%struct.OptValue = type { i32, i32, [256 x i8], i32, double }
%struct.ArgTestCase = type { ptr, i32, [16 x ptr], i32, i32, i32, i32 }
%struct.OptSpec = type { [64 x i8], i8, i32, i32, [256 x i8], [8 x ptr], i32, i32, double, ptr }

$printf = comdat any

$fprintf = comdat any

$__local_stdio_printf_options = comdat any

$snprintf = comdat any

$"??_C@_0BB@PINFDMBC@Parse?5error?3?5?$CFs?6?$AA@" = comdat any

$"??_C@_0BA@BJCMKHMP@Checksum?3?5?$CF08x?6?$AA@" = comdat any

$"??_C@_0BD@FLBPABFM@?6?9?9?9?5Test?3?5?$CFs?5?9?9?9?6?$AA@" = comdat any

$"??_C@_0CI@KCDJLGBM@?5?5FAIL?3?5expected?5error?5but?5got?5s@" = comdat any

$"??_C@_0BK@HAOANDLD@?5?5Got?5expected?5error?3?5?$CFs?6?$AA@" = comdat any

$"??_C@_0BO@LHNBMFPM@?5?5FAIL?3?5unexpected?5error?3?5?$CFs?6?$AA@" = comdat any

$"??_C@_07KBAMPNJK@verbose?$AA@" = comdat any

$"??_C@_0CE@HEBBBNLJ@?5?5FAIL?3?5verbose?5expected?5?$CFd?5got?5@" = comdat any

$"??_C@_05IOMEMJEC@count?$AA@" = comdat any

$"??_C@_0CC@HJLPILOP@?5?5FAIL?3?5count?5expected?5?$CFd?5got?5?$CFd@" = comdat any

$"??_C@_0CH@BIIEMMAB@?5?5FAIL?3?5positional?5expected?5?$CFd?5g@" = comdat any

$"??_C@_0BC@OBGJKJBN@?5?5Checksum?3?5?$CF08x?6?$AA@" = comdat any

$"??_C@_0BF@PPBDCJJJ@Tests?3?5?$CFd?1?$CFd?5passed?6?$AA@" = comdat any

$"??_C@_0BG@LCHFBCCD@Total?5checksum?3?5?$CF08x?6?$AA@" = comdat any

$"??_C@_02BAABKJLB@?9?9?$AA@" = comdat any

$"??_C@_0BF@CDEDJOM@Unknown?5option?3?5?9?9?$CFs?$AA@" = comdat any

$"??_C@_0BE@DIJGHPHO@Unknown?5option?3?5?9?$CFc?$AA@" = comdat any

$"??_C@_06DJDOIFHB@stdout?$AA@" = comdat any

$"??_C@_04CLCAKCEI@json?$AA@" = comdat any

$"??_C@_03NIPKHAHH@csv?$AA@" = comdat any

$"??_C@_04CIMGMMMG@text?$AA@" = comdat any

$"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@" = comdat any

$"??_C@_0BM@JBPENILB@Invalid?5value?5?8?$CFs?8?5for?5?9?9?$CFs?$AA@" = comdat any

$"??_C@_0P@BLENMIJJ@?5?5?9?9?$CFs?5?$CI?9?$CFc?$CJ?3?5?$AA@" = comdat any

$"??_C@_0L@CIMKDCEL@?$FLdefault?$FN?5?$AA@" = comdat any

$"??_C@_04LOAJBDKD@true?$AA@" = comdat any

$"??_C@_05LAPONLG@false?$AA@" = comdat any

$"??_C@_05LFNECPGD@?$CC?$CFs?$CC?6?$AA@" = comdat any

$"??_C@_03PMGGPEJJ@?$CFd?6?$AA@" = comdat any

$"??_C@_05KOFBGLFC@?$CF?44f?6?$AA@" = comdat any

$"??_C@_0BC@ONCCKKPK@Positional?5?$CI?$CFd?$CJ?3?6?$AA@" = comdat any

$"??_C@_0L@CJLEEMNJ@?5?5?$FL?$CFd?$FN?5?$CFs?6?$AA@" = comdat any

$"??_C@_0N@IGBPAEML@No?5arguments?$AA@" = comdat any

$"??_C@_04GIAOPBEH@prog?$AA@" = comdat any

$"??_C@_0L@KBILEBDD@Long?5flags?$AA@" = comdat any

$"??_C@_09OOLJKHBB@?9?9verbose?$AA@" = comdat any

$"??_C@_07HNNKNCGP@?9?9debug?$AA@" = comdat any

$"??_C@_0BE@DDFAOGFO@Short?5flags?5bundled?$AA@" = comdat any

$"??_C@_03NNAOONEB@?9vd?$AA@" = comdat any

$"??_C@_0BD@GADMFHGJ@Long?5option?5with?5?$DN?$AA@" = comdat any

$"??_C@_0BC@NBKDBOKO@?9?9output?$DNfile?4txt?$AA@" = comdat any

$"??_C@_0L@EDFJOFDN@?9?9count?$DN42?$AA@" = comdat any

$"??_C@_0BI@KOANHKK@Short?5option?5with?5value?$AA@" = comdat any

$"??_C@_02NCEOIEDG@?9o?$AA@" = comdat any

$"??_C@_07HFAGMOAO@out?4dat?$AA@" = comdat any

$"??_C@_02MLFFLFHH@?9n?$AA@" = comdat any

$"??_C@_02PKFBJPLJ@10?$AA@" = comdat any

$"??_C@_0BL@IFJCPLLI@Mixed?5flags?5and?5positional?$AA@" = comdat any

$"??_C@_02EJEOCNCO@?9v?$AA@" = comdat any

$"??_C@_09OBMADHKJ@?9?9count?$DN5?$AA@" = comdat any

$"??_C@_09CKLNPFEL@file1?4txt?$AA@" = comdat any

$"??_C@_09KMCJIHOF@file2?4txt?$AA@" = comdat any

$"??_C@_09GHHFFEEA@file3?4txt?$AA@" = comdat any

$"??_C@_0BG@NKCDMKAA@Double?5dash?5separator?$AA@" = comdat any

$"??_C@_0BA@DBNJCLAF@?9?9not?9an?9option?$AA@" = comdat any

$"??_C@_03PCBNCGFH@pos?$AA@" = comdat any

$"??_C@_0BK@NFPDDMGH@Format?5validation?5?$CIvalid?$CJ?$AA@" = comdat any

$"??_C@_0O@FFCENHLE@?9?9format?$DNjson?$AA@" = comdat any

$"??_C@_09OPGGCPBK@input?4dat?$AA@" = comdat any

$"??_C@_0BM@CAILGFBO@Format?5validation?5?$CIinvalid?$CJ?$AA@" = comdat any

$"??_C@_0N@EMHOJFEF@?9?9format?$DNxml?$AA@" = comdat any

$"??_C@_0P@MEBGGHOD@Unknown?5option?$AA@" = comdat any

$"??_C@_0O@JOIAGPGK@?9?9nonexistent?$AA@" = comdat any

$"??_C@_0N@ONNOCOMC@Float?5option?$AA@" = comdat any

$"??_C@_0BB@EJCPPMND@?9?9threshold?$DN0?475?$AA@" = comdat any

@"??_C@_0BB@PINFDMBC@Parse?5error?3?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [17 x i8] c"Parse error: %s\0A\00", comdat, align 1
@"??_C@_0BA@BJCMKHMP@Checksum?3?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [16 x i8] c"Checksum: %08x\0A\00", comdat, align 1
@"??_C@_0BD@FLBPABFM@?6?9?9?9?5Test?3?5?$CFs?5?9?9?9?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [19 x i8] c"\0A--- Test: %s ---\0A\00", comdat, align 1
@"??_C@_0CI@KCDJLGBM@?5?5FAIL?3?5expected?5error?5but?5got?5s@" = linkonce_odr dso_local unnamed_addr constant [40 x i8] c"  FAIL: expected error but got success\0A\00", comdat, align 1
@"??_C@_0BK@HAOANDLD@?5?5Got?5expected?5error?3?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [26 x i8] c"  Got expected error: %s\0A\00", comdat, align 1
@"??_C@_0BO@LHNBMFPM@?5?5FAIL?3?5unexpected?5error?3?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [30 x i8] c"  FAIL: unexpected error: %s\0A\00", comdat, align 1
@"??_C@_07KBAMPNJK@verbose?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"verbose\00", comdat, align 1
@"??_C@_0CE@HEBBBNLJ@?5?5FAIL?3?5verbose?5expected?5?$CFd?5got?5@" = linkonce_odr dso_local unnamed_addr constant [36 x i8] c"  FAIL: verbose expected %d got %d\0A\00", comdat, align 1
@"??_C@_05IOMEMJEC@count?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"count\00", comdat, align 1
@"??_C@_0CC@HJLPILOP@?5?5FAIL?3?5count?5expected?5?$CFd?5got?5?$CFd@" = linkonce_odr dso_local unnamed_addr constant [34 x i8] c"  FAIL: count expected %d got %d\0A\00", comdat, align 1
@"??_C@_0CH@BIIEMMAB@?5?5FAIL?3?5positional?5expected?5?$CFd?5g@" = linkonce_odr dso_local unnamed_addr constant [39 x i8] c"  FAIL: positional expected %d got %d\0A\00", comdat, align 1
@"??_C@_0BC@OBGJKJBN@?5?5Checksum?3?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [18 x i8] c"  Checksum: %08x\0A\00", comdat, align 1
@"??_C@_0BF@PPBDCJJJ@Tests?3?5?$CFd?1?$CFd?5passed?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [21 x i8] c"Tests: %d/%d passed\0A\00", comdat, align 1
@"??_C@_0BG@LCHFBCCD@Total?5checksum?3?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [22 x i8] c"Total checksum: %08x\0A\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@"??_C@_02BAABKJLB@?9?9?$AA@" = linkonce_odr dso_local unnamed_addr constant [3 x i8] c"--\00", comdat, align 1
@"??_C@_0BF@CDEDJOM@Unknown?5option?3?5?9?9?$CFs?$AA@" = linkonce_odr dso_local unnamed_addr constant [21 x i8] c"Unknown option: --%s\00", comdat, align 1
@"??_C@_0BE@DIJGHPHO@Unknown?5option?3?5?9?$CFc?$AA@" = linkonce_odr dso_local unnamed_addr constant [20 x i8] c"Unknown option: -%c\00", comdat, align 1
@"??_C@_06DJDOIFHB@stdout?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"stdout\00", comdat, align 1
@"??_C@_04CLCAKCEI@json?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"json\00", comdat, align 1
@"??_C@_03NIPKHAHH@csv?$AA@" = linkonce_odr dso_local unnamed_addr constant [4 x i8] c"csv\00", comdat, align 1
@"??_C@_04CIMGMMMG@text?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"text\00", comdat, align 1
@schema = internal constant [7 x { [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr }] [{ [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr } { [64 x i8] c"verbose\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i8 118, [3 x i8] zeroinitializer, i32 0, i32 0, [256 x i8] c"Enable verbose output\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [4 x i8] zeroinitializer, [8 x ptr] zeroinitializer, i32 0, i32 0, double 0.000000e+00, ptr null }, { [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr } { [64 x i8] c"output\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i8 111, [3 x i8] zeroinitializer, i32 1, i32 0, [256 x i8] c"Output file path\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [4 x i8] zeroinitializer, [8 x ptr] zeroinitializer, i32 0, i32 0, double 0.000000e+00, ptr @"??_C@_06DJDOIFHB@stdout?$AA@" }, { [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr } { [64 x i8] c"count\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i8 110, [3 x i8] zeroinitializer, i32 2, i32 0, [256 x i8] c"Number of iterations\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [4 x i8] zeroinitializer, [8 x ptr] zeroinitializer, i32 0, i32 1, double 0.000000e+00, ptr null }, { [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr } { [64 x i8] c"threshold\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i8 116, [3 x i8] zeroinitializer, i32 3, i32 0, [256 x i8] c"Threshold value\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [4 x i8] zeroinitializer, [8 x ptr] zeroinitializer, i32 0, i32 0, double 5.000000e-01, ptr null }, { [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr } { [64 x i8] c"help\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i8 104, [3 x i8] zeroinitializer, i32 0, i32 0, [256 x i8] c"Show help\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [4 x i8] zeroinitializer, [8 x ptr] zeroinitializer, i32 0, i32 0, double 0.000000e+00, ptr null }, { [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr } { [64 x i8] c"debug\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i8 100, [3 x i8] zeroinitializer, i32 0, i32 0, [256 x i8] c"Enable debug mode\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [4 x i8] zeroinitializer, [8 x ptr] zeroinitializer, i32 0, i32 0, double 0.000000e+00, ptr null }, { [64 x i8], i8, [3 x i8], i32, i32, [256 x i8], [4 x i8], [8 x ptr], i32, i32, double, ptr } { [64 x i8] c"format\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", i8 102, [3 x i8] zeroinitializer, i32 1, i32 0, [256 x i8] c"Output format\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", [4 x i8] zeroinitializer, [8 x ptr] [ptr @"??_C@_04CLCAKCEI@json?$AA@", ptr @"??_C@_03NIPKHAHH@csv?$AA@", ptr @"??_C@_04CIMGMMMG@text?$AA@", ptr null, ptr null, ptr null, ptr null, ptr null], i32 3, i32 0, double 0.000000e+00, ptr @"??_C@_04CIMGMMMG@text?$AA@" }], align 16
@"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@" = linkonce_odr dso_local unnamed_addr constant [29 x i8] c"Option --%s requires a value\00", comdat, align 1
@"??_C@_0BM@JBPENILB@Invalid?5value?5?8?$CFs?8?5for?5?9?9?$CFs?$AA@" = linkonce_odr dso_local unnamed_addr constant [28 x i8] c"Invalid value '%s' for --%s\00", comdat, align 1
@"??_C@_0P@BLENMIJJ@?5?5?9?9?$CFs?5?$CI?9?$CFc?$CJ?3?5?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"  --%s (-%c): \00", comdat, align 1
@"??_C@_0L@CIMKDCEL@?$FLdefault?$FN?5?$AA@" = linkonce_odr dso_local unnamed_addr constant [11 x i8] c"[default] \00", comdat, align 1
@"??_C@_04LOAJBDKD@true?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"true\00", comdat, align 1
@"??_C@_05LAPONLG@false?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"false\00", comdat, align 1
@"??_C@_05LFNECPGD@?$CC?$CFs?$CC?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"\22%s\22\0A\00", comdat, align 1
@"??_C@_03PMGGPEJJ@?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [4 x i8] c"%d\0A\00", comdat, align 1
@"??_C@_05KOFBGLFC@?$CF?44f?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"%.4f\0A\00", comdat, align 1
@"??_C@_0BC@ONCCKKPK@Positional?5?$CI?$CFd?$CJ?3?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [18 x i8] c"Positional (%d):\0A\00", comdat, align 1
@"??_C@_0L@CJLEEMNJ@?5?5?$FL?$CFd?$FN?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [11 x i8] c"  [%d] %s\0A\00", comdat, align 1
@"??_C@_0N@IGBPAEML@No?5arguments?$AA@" = linkonce_odr dso_local unnamed_addr constant [13 x i8] c"No arguments\00", comdat, align 1
@"??_C@_04GIAOPBEH@prog?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"prog\00", comdat, align 1
@"??_C@_0L@KBILEBDD@Long?5flags?$AA@" = linkonce_odr dso_local unnamed_addr constant [11 x i8] c"Long flags\00", comdat, align 1
@"??_C@_09OOLJKHBB@?9?9verbose?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"--verbose\00", comdat, align 1
@"??_C@_07HNNKNCGP@?9?9debug?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"--debug\00", comdat, align 1
@"??_C@_0BE@DDFAOGFO@Short?5flags?5bundled?$AA@" = linkonce_odr dso_local unnamed_addr constant [20 x i8] c"Short flags bundled\00", comdat, align 1
@"??_C@_03NNAOONEB@?9vd?$AA@" = linkonce_odr dso_local unnamed_addr constant [4 x i8] c"-vd\00", comdat, align 1
@"??_C@_0BD@GADMFHGJ@Long?5option?5with?5?$DN?$AA@" = linkonce_odr dso_local unnamed_addr constant [19 x i8] c"Long option with =\00", comdat, align 1
@"??_C@_0BC@NBKDBOKO@?9?9output?$DNfile?4txt?$AA@" = linkonce_odr dso_local unnamed_addr constant [18 x i8] c"--output=file.txt\00", comdat, align 1
@"??_C@_0L@EDFJOFDN@?9?9count?$DN42?$AA@" = linkonce_odr dso_local unnamed_addr constant [11 x i8] c"--count=42\00", comdat, align 1
@"??_C@_0BI@KOANHKK@Short?5option?5with?5value?$AA@" = linkonce_odr dso_local unnamed_addr constant [24 x i8] c"Short option with value\00", comdat, align 1
@"??_C@_02NCEOIEDG@?9o?$AA@" = linkonce_odr dso_local unnamed_addr constant [3 x i8] c"-o\00", comdat, align 1
@"??_C@_07HFAGMOAO@out?4dat?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"out.dat\00", comdat, align 1
@"??_C@_02MLFFLFHH@?9n?$AA@" = linkonce_odr dso_local unnamed_addr constant [3 x i8] c"-n\00", comdat, align 1
@"??_C@_02PKFBJPLJ@10?$AA@" = linkonce_odr dso_local unnamed_addr constant [3 x i8] c"10\00", comdat, align 1
@"??_C@_0BL@IFJCPLLI@Mixed?5flags?5and?5positional?$AA@" = linkonce_odr dso_local unnamed_addr constant [27 x i8] c"Mixed flags and positional\00", comdat, align 1
@"??_C@_02EJEOCNCO@?9v?$AA@" = linkonce_odr dso_local unnamed_addr constant [3 x i8] c"-v\00", comdat, align 1
@"??_C@_09OBMADHKJ@?9?9count?$DN5?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"--count=5\00", comdat, align 1
@"??_C@_09CKLNPFEL@file1?4txt?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"file1.txt\00", comdat, align 1
@"??_C@_09KMCJIHOF@file2?4txt?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"file2.txt\00", comdat, align 1
@"??_C@_09GHHFFEEA@file3?4txt?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"file3.txt\00", comdat, align 1
@"??_C@_0BG@NKCDMKAA@Double?5dash?5separator?$AA@" = linkonce_odr dso_local unnamed_addr constant [22 x i8] c"Double dash separator\00", comdat, align 1
@"??_C@_0BA@DBNJCLAF@?9?9not?9an?9option?$AA@" = linkonce_odr dso_local unnamed_addr constant [16 x i8] c"--not-an-option\00", comdat, align 1
@"??_C@_03PCBNCGFH@pos?$AA@" = linkonce_odr dso_local unnamed_addr constant [4 x i8] c"pos\00", comdat, align 1
@"??_C@_0BK@NFPDDMGH@Format?5validation?5?$CIvalid?$CJ?$AA@" = linkonce_odr dso_local unnamed_addr constant [26 x i8] c"Format validation (valid)\00", comdat, align 1
@"??_C@_0O@FFCENHLE@?9?9format?$DNjson?$AA@" = linkonce_odr dso_local unnamed_addr constant [14 x i8] c"--format=json\00", comdat, align 1
@"??_C@_09OPGGCPBK@input?4dat?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"input.dat\00", comdat, align 1
@"??_C@_0BM@CAILGFBO@Format?5validation?5?$CIinvalid?$CJ?$AA@" = linkonce_odr dso_local unnamed_addr constant [28 x i8] c"Format validation (invalid)\00", comdat, align 1
@"??_C@_0N@EMHOJFEF@?9?9format?$DNxml?$AA@" = linkonce_odr dso_local unnamed_addr constant [13 x i8] c"--format=xml\00", comdat, align 1
@"??_C@_0P@MEBGGHOD@Unknown?5option?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"Unknown option\00", comdat, align 1
@"??_C@_0O@JOIAGPGK@?9?9nonexistent?$AA@" = linkonce_odr dso_local unnamed_addr constant [14 x i8] c"--nonexistent\00", comdat, align 1
@"??_C@_0N@ONNOCOMC@Float?5option?$AA@" = linkonce_odr dso_local unnamed_addr constant [13 x i8] c"Float option\00", comdat, align 1
@"??_C@_0BB@EJCPPMND@?9?9threshold?$DN0?475?$AA@" = linkonce_odr dso_local unnamed_addr constant [17 x i8] c"--threshold=0.75\00", comdat, align 1
@arg_tests = internal constant <{ { ptr, i32, [4 x i8], <{ ptr, [15 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, [14 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, ptr, ptr, [11 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, ptr, ptr, ptr, [10 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, ptr, ptr, [11 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, [14 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, [14 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 }, { ptr, i32, [4 x i8], [16 x ptr], i32, i32, i32, i32 } }> <{ { ptr, i32, [4 x i8], <{ ptr, [15 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0N@IGBPAEML@No?5arguments?$AA@", i32 1, [4 x i8] zeroinitializer, <{ ptr, [15 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", [15 x ptr] zeroinitializer }>, i32 0, i32 0, i32 1, i32 0 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0L@KBILEBDD@Long?5flags?$AA@", i32 3, [4 x i8] zeroinitializer, <{ ptr, ptr, ptr, [13 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_09OOLJKHBB@?9?9verbose?$AA@", ptr @"??_C@_07HNNKNCGP@?9?9debug?$AA@", [13 x ptr] zeroinitializer }>, i32 0, i32 1, i32 1, i32 0 }, { ptr, i32, [4 x i8], <{ ptr, ptr, [14 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0BE@DDFAOGFO@Short?5flags?5bundled?$AA@", i32 2, [4 x i8] zeroinitializer, <{ ptr, ptr, [14 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_03NNAOONEB@?9vd?$AA@", [14 x ptr] zeroinitializer }>, i32 0, i32 1, i32 1, i32 0 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0BD@GADMFHGJ@Long?5option?5with?5?$DN?$AA@", i32 3, [4 x i8] zeroinitializer, <{ ptr, ptr, ptr, [13 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_0BC@NBKDBOKO@?9?9output?$DNfile?4txt?$AA@", ptr @"??_C@_0L@EDFJOFDN@?9?9count?$DN42?$AA@", [13 x ptr] zeroinitializer }>, i32 0, i32 0, i32 42, i32 0 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, ptr, ptr, [11 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0BI@KOANHKK@Short?5option?5with?5value?$AA@", i32 5, [4 x i8] zeroinitializer, <{ ptr, ptr, ptr, ptr, ptr, [11 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_02NCEOIEDG@?9o?$AA@", ptr @"??_C@_07HFAGMOAO@out?4dat?$AA@", ptr @"??_C@_02MLFFLFHH@?9n?$AA@", ptr @"??_C@_02PKFBJPLJ@10?$AA@", [11 x ptr] zeroinitializer }>, i32 0, i32 0, i32 10, i32 0 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, ptr, ptr, ptr, [10 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0BL@IFJCPLLI@Mixed?5flags?5and?5positional?$AA@", i32 6, [4 x i8] zeroinitializer, <{ ptr, ptr, ptr, ptr, ptr, ptr, [10 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_02EJEOCNCO@?9v?$AA@", ptr @"??_C@_09OBMADHKJ@?9?9count?$DN5?$AA@", ptr @"??_C@_09CKLNPFEL@file1?4txt?$AA@", ptr @"??_C@_09KMCJIHOF@file2?4txt?$AA@", ptr @"??_C@_09GHHFFEEA@file3?4txt?$AA@", [10 x ptr] zeroinitializer }>, i32 0, i32 1, i32 5, i32 3 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, ptr, ptr, [11 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0BG@NKCDMKAA@Double?5dash?5separator?$AA@", i32 5, [4 x i8] zeroinitializer, <{ ptr, ptr, ptr, ptr, ptr, [11 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_09OOLJKHBB@?9?9verbose?$AA@", ptr @"??_C@_02BAABKJLB@?9?9?$AA@", ptr @"??_C@_0BA@DBNJCLAF@?9?9not?9an?9option?$AA@", ptr @"??_C@_03PCBNCGFH@pos?$AA@", [11 x ptr] zeroinitializer }>, i32 0, i32 1, i32 1, i32 2 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0BK@NFPDDMGH@Format?5validation?5?$CIvalid?$CJ?$AA@", i32 3, [4 x i8] zeroinitializer, <{ ptr, ptr, ptr, [13 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_0O@FFCENHLE@?9?9format?$DNjson?$AA@", ptr @"??_C@_09OPGGCPBK@input?4dat?$AA@", [13 x ptr] zeroinitializer }>, i32 0, i32 0, i32 1, i32 1 }, { ptr, i32, [4 x i8], <{ ptr, ptr, [14 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0BM@CAILGFBO@Format?5validation?5?$CIinvalid?$CJ?$AA@", i32 2, [4 x i8] zeroinitializer, <{ ptr, ptr, [14 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_0N@EMHOJFEF@?9?9format?$DNxml?$AA@", [14 x ptr] zeroinitializer }>, i32 1, i32 0, i32 1, i32 0 }, { ptr, i32, [4 x i8], <{ ptr, ptr, [14 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0P@MEBGGHOD@Unknown?5option?$AA@", i32 2, [4 x i8] zeroinitializer, <{ ptr, ptr, [14 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_0O@JOIAGPGK@?9?9nonexistent?$AA@", [14 x ptr] zeroinitializer }>, i32 1, i32 0, i32 1, i32 0 }, { ptr, i32, [4 x i8], <{ ptr, ptr, ptr, [13 x ptr] }>, i32, i32, i32, i32 } { ptr @"??_C@_0N@ONNOCOMC@Float?5option?$AA@", i32 3, [4 x i8] zeroinitializer, <{ ptr, ptr, ptr, [13 x ptr] }> <{ ptr @"??_C@_04GIAOPBEH@prog?$AA@", ptr @"??_C@_0BB@EJCPPMND@?9?9threshold?$DN0?475?$AA@", ptr @"??_C@_02EJEOCNCO@?9v?$AA@", [13 x ptr] zeroinitializer }>, i32 0, i32 1, i32 1, i32 0 }, { ptr, i32, [4 x i8], [16 x ptr], i32, i32, i32, i32 } zeroinitializer }>, align 16
@str = private unnamed_addr constant [27 x i8] c"=== Argparse Benchmark ===\00", align 1
@str.3 = private unnamed_addr constant [17 x i8] c"\0A=== Summary ===\00", align 1
@str.5 = private unnamed_addr constant [13 x i8] c"RESULT: FAIL\00", align 1
@str.6 = private unnamed_addr constant [13 x i8] c"  Test: FAIL\00", align 1
@str.7 = private unnamed_addr constant [13 x i8] c"  Test: PASS\00", align 1
@str.8 = private unnamed_addr constant [13 x i8] c"RESULT: PASS\00", align 1
@str.9 = private unnamed_addr constant [9 x i8] c"Options:\00", align 1

; Function Attrs: nounwind uwtable
define dso_local range(i32 0, 2) i32 @main(i32 noundef %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #0 {
  %3 = alloca %struct.ParseResult, align 8
  %4 = alloca %struct.ParseResult, align 8
  %5 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %6 = icmp sgt i32 %0, 1
  br i1 %6, label %10, label %7

7:                                                ; preds = %2
  %8 = getelementptr inbounds nuw i8, ptr %4, i64 12684
  %9 = getelementptr inbounds nuw i8, ptr %4, i64 12676
  br label %28

10:                                               ; preds = %2
  call void @llvm.lifetime.start.p0(ptr nonnull %3) #15
  %11 = call fastcc i32 @parse_args(i32 noundef %0, ptr noundef %1, ptr noundef %3)
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %17, label %13

13:                                               ; preds = %10
  %14 = getelementptr inbounds nuw i8, ptr %3, i64 12684
  %15 = call ptr @__acrt_iob_func(i32 noundef 2) #15
  %16 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %15, ptr noundef nonnull @"??_C@_0BB@PINFDMBC@Parse?5error?3?5?$CFs?6?$AA@", ptr noundef nonnull %14) #15
  br label %21

17:                                               ; preds = %10
  call fastcc void @print_result(ptr noundef %3)
  %18 = call fastcc i32 @result_checksum(ptr noundef %3)
  %19 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BA@BJCMKHMP@Checksum?3?5?$CF08x?6?$AA@", i32 noundef %18)
  %20 = call i32 @puts(ptr nonnull dereferenceable(1) @str.8)
  br label %21

21:                                               ; preds = %17, %13
  %22 = phi i32 [ 1, %13 ], [ 0, %17 ]
  call void @llvm.lifetime.end.p0(ptr nonnull %3) #15
  br label %110

23:                                               ; preds = %101
  %24 = call i32 @puts(ptr nonnull dereferenceable(1) @str.3)
  %25 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BF@PPBDCJJJ@Tests?3?5?$CFd?1?$CFd?5passed?6?$AA@", i32 noundef %104, i32 noundef 11)
  %26 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BG@LCHFBCCD@Total?5checksum?3?5?$CF08x?6?$AA@", i32 noundef %102)
  %27 = icmp eq i32 %103, 0
  br i1 %27, label %106, label %108

28:                                               ; preds = %7, %101
  %29 = phi i64 [ 0, %7 ], [ %35, %101 ]
  %30 = phi i32 [ 0, %7 ], [ %104, %101 ]
  %31 = phi i32 [ 0, %7 ], [ %103, %101 ]
  %32 = phi i32 [ 0, %7 ], [ %102, %101 ]
  %33 = getelementptr inbounds nuw %struct.ArgTestCase, ptr @arg_tests, i64 %29
  %34 = load ptr, ptr %33, align 16
  %35 = add nuw nsw i64 %29, 1
  %36 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BD@FLBPABFM@?6?9?9?9?5Test?3?5?$CFs?5?9?9?9?6?$AA@", ptr noundef %34)
  call void @llvm.lifetime.start.p0(ptr nonnull %4) #15
  %37 = getelementptr inbounds nuw i8, ptr %33, i64 16
  %38 = getelementptr inbounds nuw i8, ptr %33, i64 8
  %39 = load i32, ptr %38, align 8
  %40 = call fastcc i32 @parse_args(i32 noundef %39, ptr noundef nonnull %37, ptr noundef %4)
  %41 = trunc i64 %29 to i32
  %42 = add i32 %41, -10
  %43 = icmp ult i32 %42, -2
  %44 = icmp eq i32 %40, 0
  br i1 %43, label %52, label %45

45:                                               ; preds = %28
  br i1 %44, label %46, label %49

46:                                               ; preds = %45
  %47 = call ptr @__acrt_iob_func(i32 noundef 2) #15
  %48 = call i64 @fwrite(ptr nonnull @"??_C@_0CI@KCDJLGBM@?5?5FAIL?3?5expected?5error?5but?5got?5s@", i64 39, i64 1, ptr %47)
  br label %101

49:                                               ; preds = %45
  %50 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BK@HAOANDLD@?5?5Got?5expected?5error?3?5?$CFs?6?$AA@", ptr noundef nonnull %8)
  %51 = add nsw i32 %30, 1
  br label %101

52:                                               ; preds = %28
  br i1 %44, label %56, label %53

53:                                               ; preds = %52
  %54 = call ptr @__acrt_iob_func(i32 noundef 2) #15
  %55 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %54, ptr noundef nonnull @"??_C@_0BO@LHNBMFPM@?5?5FAIL?3?5unexpected?5error?3?5?$CFs?6?$AA@", ptr noundef nonnull %8) #15
  br label %101

56:                                               ; preds = %52
  %57 = call fastcc i32 @find_long_option(ptr noundef nonnull @"??_C@_07KBAMPNJK@verbose?$AA@")
  %58 = sext i32 %57 to i64
  %59 = getelementptr inbounds %struct.OptValue, ptr %4, i64 %58
  %60 = getelementptr inbounds nuw i8, ptr %59, i64 4
  %61 = load i32, ptr %60, align 4
  %62 = getelementptr inbounds nuw i8, ptr %33, i64 148
  %63 = load i32, ptr %62, align 4
  %64 = icmp eq i32 %61, %63
  br i1 %64, label %68, label %65

65:                                               ; preds = %56
  %66 = call ptr @__acrt_iob_func(i32 noundef 2) #15
  %67 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %66, ptr noundef nonnull @"??_C@_0CE@HEBBBNLJ@?5?5FAIL?3?5verbose?5expected?5?$CFd?5got?5@", i32 noundef %63, i32 noundef %61) #15
  br label %68

68:                                               ; preds = %65, %56
  %69 = phi i32 [ 0, %65 ], [ 1, %56 ]
  %70 = call fastcc i32 @find_long_option(ptr noundef nonnull @"??_C@_05IOMEMJEC@count?$AA@")
  %71 = sext i32 %70 to i64
  %72 = getelementptr inbounds %struct.OptValue, ptr %4, i64 %71
  %73 = getelementptr inbounds nuw i8, ptr %72, i64 264
  %74 = load i32, ptr %73, align 8
  %75 = getelementptr inbounds nuw i8, ptr %33, i64 152
  %76 = load i32, ptr %75, align 8
  %77 = icmp eq i32 %74, %76
  br i1 %77, label %81, label %78

78:                                               ; preds = %68
  %79 = call ptr @__acrt_iob_func(i32 noundef 2) #15
  %80 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %79, ptr noundef nonnull @"??_C@_0CC@HJLPILOP@?5?5FAIL?3?5count?5expected?5?$CFd?5got?5?$CFd@", i32 noundef %76, i32 noundef %74) #15
  br label %81

81:                                               ; preds = %78, %68
  %82 = phi i32 [ 0, %78 ], [ %69, %68 ]
  %83 = load i32, ptr %9, align 4
  %84 = getelementptr inbounds nuw i8, ptr %33, i64 156
  %85 = load i32, ptr %84, align 4
  %86 = icmp eq i32 %83, %85
  br i1 %86, label %90, label %87

87:                                               ; preds = %81
  %88 = call ptr @__acrt_iob_func(i32 noundef 2) #15
  %89 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %88, ptr noundef nonnull @"??_C@_0CH@BIIEMMAB@?5?5FAIL?3?5positional?5expected?5?$CFd?5g@", i32 noundef %85, i32 noundef %83) #15
  br label %90

90:                                               ; preds = %87, %81
  %91 = phi i32 [ 0, %87 ], [ %82, %81 ]
  call fastcc void @print_result(ptr noundef %4)
  %92 = call fastcc i32 @result_checksum(ptr noundef %4)
  %93 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BC@OBGJKJBN@?5?5Checksum?3?5?$CF08x?6?$AA@", i32 noundef %92)
  %94 = xor i32 %92, %32
  %95 = icmp eq i32 %91, 0
  br i1 %95, label %99, label %96

96:                                               ; preds = %90
  %97 = call i32 @puts(ptr nonnull dereferenceable(1) @str.7)
  %98 = add nsw i32 %30, 1
  br label %101

99:                                               ; preds = %90
  %100 = call i32 @puts(ptr nonnull dereferenceable(1) @str.6)
  br label %101

101:                                              ; preds = %96, %99, %53, %49, %46
  %102 = phi i32 [ %32, %46 ], [ %32, %49 ], [ %32, %53 ], [ %94, %99 ], [ %94, %96 ]
  %103 = phi i32 [ 1, %46 ], [ %31, %49 ], [ 1, %53 ], [ 1, %99 ], [ %31, %96 ]
  %104 = phi i32 [ %30, %46 ], [ %51, %49 ], [ %30, %53 ], [ %30, %99 ], [ %98, %96 ]
  call void @llvm.lifetime.end.p0(ptr nonnull %4) #15
  %105 = icmp eq i64 %35, 11
  br i1 %105, label %23, label %28, !llvm.loop !8

106:                                              ; preds = %23
  %107 = call i32 @puts(ptr nonnull dereferenceable(1) @str.8)
  br label %110

108:                                              ; preds = %23
  %109 = call i32 @puts(ptr nonnull dereferenceable(1) @str.5)
  br label %110

110:                                              ; preds = %106, %108, %21
  %111 = phi i32 [ %22, %21 ], [ 1, %108 ], [ 0, %106 ]
  ret i32 %111
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @printf(ptr noundef %0, ...) local_unnamed_addr #1 comdat {
  %2 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %2) #15
  call void @llvm.va_start.p0(ptr nonnull %2)
  %3 = load ptr, ptr %2, align 8
  %4 = call ptr @__acrt_iob_func(i32 noundef 1) #15
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %4, ptr noundef %0, ptr noundef null, ptr noundef %3) #15
  call void @llvm.va_end.p0(ptr nonnull %2)
  call void @llvm.lifetime.end.p0(ptr nonnull %2) #15
  ret i32 %7
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #2

; Function Attrs: nounwind uwtable
define internal fastcc range(i32 -1, 1) i32 @parse_args(i32 noundef %0, ptr noundef readonly captures(none) %1, ptr noundef nonnull initializes((0, 12944)) %2) unnamed_addr #0 {
  %4 = alloca [64 x i8], align 16
  tail call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(12944) %2, i8 0, i64 12944, i1 false)
  %5 = getelementptr inbounds nuw i8, ptr %2, i64 4480
  store i32 7, ptr %5, align 8
  br label %6

6:                                                ; preds = %36, %3
  %7 = phi i64 [ 0, %3 ], [ %37, %36 ]
  %8 = getelementptr inbounds nuw %struct.OptSpec, ptr @schema, i64 %7
  %9 = getelementptr inbounds nuw i8, ptr %8, i64 404
  %10 = load i32, ptr %9, align 4
  %11 = getelementptr inbounds nuw %struct.OptValue, ptr %2, i64 %7
  %12 = getelementptr inbounds nuw i8, ptr %11, i64 264
  store i32 %10, ptr %12, align 8
  %13 = getelementptr inbounds nuw i8, ptr %8, i64 408
  %14 = load double, ptr %13, align 8
  %15 = getelementptr inbounds nuw i8, ptr %11, i64 272
  store double %14, ptr %15, align 8
  %16 = trunc nuw nsw i64 %7 to i32
  switch i32 %16, label %36 [
    i32 6, label %17
    i32 1, label %17
  ]

17:                                               ; preds = %6, %6
  %18 = getelementptr inbounds nuw i8, ptr %8, i64 416
  %19 = load ptr, ptr %18, align 8
  %20 = getelementptr inbounds nuw i8, ptr %11, i64 8
  %21 = load i8, ptr %19, align 1
  %22 = icmp eq i8 %21, 0
  br i1 %22, label %33, label %23

23:                                               ; preds = %17, %23
  %24 = phi i64 [ %27, %23 ], [ 0, %17 ]
  %25 = phi i8 [ %29, %23 ], [ %21, %17 ]
  %26 = getelementptr inbounds nuw i8, ptr %20, i64 %24
  store i8 %25, ptr %26, align 1
  %27 = add nuw nsw i64 %24, 1
  %28 = getelementptr inbounds nuw i8, ptr %19, i64 %27
  %29 = load i8, ptr %28, align 1
  %30 = icmp ne i8 %29, 0
  %31 = icmp samesign ult i64 %24, 254
  %32 = select i1 %30, i1 %31, i1 false
  br i1 %32, label %23, label %33, !llvm.loop !10

33:                                               ; preds = %23, %17
  %34 = phi i64 [ 0, %17 ], [ %27, %23 ]
  %35 = getelementptr inbounds nuw i8, ptr %20, i64 %34
  store i8 0, ptr %35, align 1
  br label %36

36:                                               ; preds = %33, %6
  %37 = add nuw nsw i64 %7, 1
  %38 = icmp eq i64 %37, 7
  br i1 %38, label %39, label %6, !llvm.loop !11

39:                                               ; preds = %36
  %40 = icmp sgt i32 %0, 1
  br i1 %40, label %41, label %194

41:                                               ; preds = %39
  %42 = getelementptr inbounds nuw i8, ptr %4, i64 63
  %43 = getelementptr inbounds nuw i8, ptr %2, i64 12684
  %44 = getelementptr inbounds nuw i8, ptr %2, i64 12680
  %45 = getelementptr inbounds nuw i8, ptr %2, i64 12676
  %46 = getelementptr inbounds nuw i8, ptr %2, i64 4484
  br label %47

47:                                               ; preds = %41, %189
  %48 = phi i32 [ 1, %41 ], [ %192, %189 ]
  %49 = phi i32 [ 0, %41 ], [ %191, %189 ]
  %50 = sext i32 %48 to i64
  %51 = getelementptr inbounds ptr, ptr %1, i64 %50
  %52 = load ptr, ptr %51, align 8
  %53 = icmp eq i32 %49, 0
  br i1 %53, label %54, label %164

54:                                               ; preds = %47
  %55 = load i8, ptr %52, align 1
  %56 = icmp eq i8 %55, 45
  br i1 %56, label %57, label %164

57:                                               ; preds = %54
  %58 = getelementptr inbounds nuw i8, ptr %52, i64 1
  %59 = load i8, ptr %58, align 1
  %60 = icmp eq i8 %59, 45
  br i1 %60, label %61, label %65

61:                                               ; preds = %57
  %62 = getelementptr inbounds nuw i8, ptr %52, i64 2
  %63 = load i8, ptr %62, align 1
  %64 = icmp eq i8 %63, 0
  br i1 %64, label %189, label %65, !llvm.loop !12

65:                                               ; preds = %61, %57
  %66 = getelementptr inbounds nuw i8, ptr %52, i64 1
  %67 = load i8, ptr %66, align 1
  switch i8 %67, label %112 [
    i8 45, label %68
    i8 0, label %164
  ]

68:                                               ; preds = %65
  %69 = getelementptr inbounds nuw i8, ptr %52, i64 2
  %70 = load i8, ptr %69, align 1
  %71 = icmp eq i8 %70, 0
  br i1 %71, label %164, label %72

72:                                               ; preds = %68
  %73 = call ptr @strchr(ptr noundef nonnull dereferenceable(1) %69, i32 noundef 61) #15
  call void @llvm.lifetime.start.p0(ptr nonnull %4) #15
  %74 = icmp eq ptr %73, null
  br i1 %74, label %85, label %75

75:                                               ; preds = %72
  %76 = ptrtoint ptr %73 to i64
  %77 = ptrtoint ptr %52 to i64
  %78 = sub i64 %76, %77
  %79 = trunc i64 %78 to i32
  %80 = add i32 %79, -2
  %81 = call i32 @llvm.smin.i32(i32 %80, i32 63)
  %82 = sext i32 %81 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %4, ptr nonnull align 1 %69, i64 %82, i1 false)
  %83 = getelementptr inbounds i8, ptr %4, i64 %82
  store i8 0, ptr %83, align 1
  %84 = getelementptr inbounds nuw i8, ptr %73, i64 1
  br label %87

85:                                               ; preds = %72
  %86 = call ptr @strncpy(ptr noundef nonnull dereferenceable(1) %4, ptr noundef nonnull dereferenceable(1) %69, i64 noundef 63) #15
  store i8 0, ptr %42, align 1
  br label %87

87:                                               ; preds = %85, %75
  %88 = phi ptr [ %84, %75 ], [ null, %85 ]
  %89 = call fastcc i32 @find_long_option(ptr noundef nonnull %4)
  %90 = icmp slt i32 %89, 0
  br i1 %90, label %91, label %93

91:                                               ; preds = %87
  %92 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %43, i64 noundef 256, ptr noundef nonnull @"??_C@_0BF@CDEDJOM@Unknown?5option?3?5?9?9?$CFs?$AA@", ptr noundef nonnull %4) #15
  store i32 1, ptr %44, align 8
  br label %187

93:                                               ; preds = %87
  %94 = zext nneg i32 %89 to i64
  %95 = shl nuw nsw i64 1, %94
  %96 = and i64 %95, 49
  %97 = icmp ne i64 %96, 0
  %98 = icmp ne ptr %88, null
  %99 = select i1 %97, i1 true, i1 %98
  br i1 %99, label %107, label %100

100:                                              ; preds = %93
  %101 = add nsw i32 %48, 1
  %102 = icmp slt i32 %101, %0
  br i1 %102, label %103, label %107

103:                                              ; preds = %100
  %104 = sext i32 %101 to i64
  %105 = getelementptr inbounds ptr, ptr %1, i64 %104
  %106 = load ptr, ptr %105, align 8
  br label %107

107:                                              ; preds = %100, %103, %93
  %108 = phi ptr [ %88, %93 ], [ %106, %103 ], [ null, %100 ]
  %109 = phi i32 [ %48, %93 ], [ %101, %103 ], [ %48, %100 ]
  %110 = call fastcc i32 @set_option(ptr noundef %2, i32 noundef %89, ptr noundef %108)
  %111 = icmp eq i32 %110, 0
  br i1 %111, label %188, label %187

112:                                              ; preds = %65, %160
  %113 = phi i8 [ %163, %160 ], [ %67, %65 ]
  %114 = phi i64 [ %161, %160 ], [ 1, %65 ]
  switch i8 %113, label %121 [
    i8 0, label %189
    i8 118, label %124
    i8 111, label %115
    i8 110, label %116
    i8 116, label %117
    i8 104, label %118
    i8 100, label %119
    i8 102, label %120
  ], !llvm.loop !12

115:                                              ; preds = %112
  br label %124

116:                                              ; preds = %112
  br label %124

117:                                              ; preds = %112
  br label %124

118:                                              ; preds = %112
  br label %124

119:                                              ; preds = %112
  br label %124

120:                                              ; preds = %112
  br label %124

121:                                              ; preds = %112
  %122 = sext i8 %113 to i32
  %123 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %43, i64 noundef 256, ptr noundef nonnull @"??_C@_0BE@DIJGHPHO@Unknown?5option?3?5?9?$CFc?$AA@", i32 noundef %122) #15
  store i32 1, ptr %44, align 8
  br label %194

124:                                              ; preds = %112, %118, %119, %115, %117, %116, %120
  %125 = phi i32 [ 6, %120 ], [ 2, %116 ], [ 3, %117 ], [ 1, %115 ], [ 5, %119 ], [ 4, %118 ], [ 0, %112 ]
  %126 = zext nneg i32 %125 to i64
  %127 = shl nuw nsw i64 1, %126
  %128 = and i64 %127, 49
  %129 = icmp eq i64 %128, 0
  br i1 %129, label %143, label %130

130:                                              ; preds = %124
  %131 = getelementptr inbounds nuw %struct.OptValue, ptr %2, i64 %126
  store i32 1, ptr %131, align 8
  %132 = getelementptr inbounds nuw %struct.OptSpec, ptr @schema, i64 %126
  %133 = getelementptr inbounds nuw i8, ptr %132, i64 68
  %134 = load i32, ptr %133, align 4
  switch i32 %134, label %160 [
    i32 0, label %135
    i32 1, label %137
    i32 2, label %139
    i32 3, label %141
  ]

135:                                              ; preds = %130
  %136 = getelementptr inbounds nuw i8, ptr %131, i64 4
  store i32 1, ptr %136, align 4
  br label %160

137:                                              ; preds = %130
  %138 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %43, i64 noundef 256, ptr noundef nonnull @"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@", ptr noundef nonnull %132) #15
  store i32 1, ptr %44, align 8
  br label %160

139:                                              ; preds = %130
  %140 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %43, i64 noundef 256, ptr noundef nonnull @"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@", ptr noundef nonnull %132) #15
  store i32 1, ptr %44, align 8
  br label %160

141:                                              ; preds = %130
  %142 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %43, i64 noundef 256, ptr noundef nonnull @"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@", ptr noundef nonnull %132) #15
  store i32 1, ptr %44, align 8
  br label %160

143:                                              ; preds = %124
  %144 = getelementptr inbounds nuw i8, ptr %52, i64 %114
  %145 = getelementptr i8, ptr %144, i64 1
  %146 = load i8, ptr %145, align 1
  %147 = icmp eq i8 %146, 0
  br i1 %147, label %148, label %155

148:                                              ; preds = %143
  %149 = add nsw i32 %48, 1
  %150 = icmp slt i32 %149, %0
  br i1 %150, label %151, label %155

151:                                              ; preds = %148
  %152 = sext i32 %149 to i64
  %153 = getelementptr inbounds ptr, ptr %1, i64 %152
  %154 = load ptr, ptr %153, align 8
  br label %155

155:                                              ; preds = %143, %148, %151
  %156 = phi i32 [ %48, %148 ], [ %149, %151 ], [ %48, %143 ]
  %157 = phi ptr [ null, %148 ], [ %154, %151 ], [ %145, %143 ]
  %158 = call fastcc i32 @set_option(ptr noundef %2, i32 noundef %125, ptr noundef %157)
  %159 = icmp eq i32 %158, 0
  br i1 %159, label %189, label %194, !llvm.loop !12

160:                                              ; preds = %141, %139, %137, %135, %130
  %161 = add nuw nsw i64 %114, 1
  %162 = getelementptr inbounds nuw i8, ptr %52, i64 %161
  %163 = load i8, ptr %162, align 1
  br label %112

164:                                              ; preds = %65, %68, %54, %47
  %165 = load i32, ptr %45, align 4
  %166 = icmp slt i32 %165, 32
  br i1 %166, label %167, label %189

167:                                              ; preds = %164
  %168 = sext i32 %165 to i64
  %169 = getelementptr inbounds [256 x i8], ptr %46, i64 %168
  %170 = load i8, ptr %52, align 1
  %171 = icmp eq i8 %170, 0
  br i1 %171, label %182, label %172

172:                                              ; preds = %167, %172
  %173 = phi i64 [ %176, %172 ], [ 0, %167 ]
  %174 = phi i8 [ %178, %172 ], [ %170, %167 ]
  %175 = getelementptr inbounds nuw i8, ptr %169, i64 %173
  store i8 %174, ptr %175, align 1
  %176 = add nuw nsw i64 %173, 1
  %177 = getelementptr inbounds nuw i8, ptr %52, i64 %176
  %178 = load i8, ptr %177, align 1
  %179 = icmp ne i8 %178, 0
  %180 = icmp samesign ult i64 %173, 254
  %181 = select i1 %179, i1 %180, i1 false
  br i1 %181, label %172, label %182, !llvm.loop !10

182:                                              ; preds = %172, %167
  %183 = phi i64 [ 0, %167 ], [ %176, %172 ]
  %184 = getelementptr inbounds nuw i8, ptr %169, i64 %183
  store i8 0, ptr %184, align 1
  %185 = load i32, ptr %45, align 4
  %186 = add nsw i32 %185, 1
  store i32 %186, ptr %45, align 4
  br label %189

187:                                              ; preds = %107, %91
  call void @llvm.lifetime.end.p0(ptr nonnull %4) #15
  br label %194

188:                                              ; preds = %107
  call void @llvm.lifetime.end.p0(ptr nonnull %4) #15
  br label %189

189:                                              ; preds = %112, %164, %182, %155, %61, %188
  %190 = phi i32 [ %109, %188 ], [ %48, %164 ], [ %48, %61 ], [ %156, %155 ], [ %48, %182 ], [ %48, %112 ]
  %191 = phi i32 [ 0, %188 ], [ %49, %164 ], [ 1, %61 ], [ 0, %155 ], [ %49, %182 ], [ 0, %112 ]
  %192 = add nsw i32 %190, 1
  %193 = icmp slt i32 %192, %0
  br i1 %193, label %47, label %194, !llvm.loop !12

194:                                              ; preds = %155, %189, %187, %39, %121
  %195 = phi i32 [ 0, %39 ], [ -1, %121 ], [ -1, %187 ], [ -1, %155 ], [ 0, %189 ]
  ret i32 %195
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @fprintf(ptr noundef %0, ptr noundef %1, ...) local_unnamed_addr #1 comdat {
  %3 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %3) #15
  call void @llvm.va_start.p0(ptr nonnull %3)
  %4 = load ptr, ptr %3, align 8
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %0, ptr noundef %1, ptr noundef null, ptr noundef %4) #15
  call void @llvm.va_end.p0(ptr nonnull %3)
  call void @llvm.lifetime.end.p0(ptr nonnull %3) #15
  ret i32 %7
}

declare dso_local ptr @__acrt_iob_func(i32 noundef) local_unnamed_addr #3

; Function Attrs: nounwind uwtable
define internal fastcc void @print_result(ptr noundef nonnull %0) unnamed_addr #0 {
  %2 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.9)
  br label %11

3:                                                ; preds = %43
  %4 = getelementptr inbounds nuw i8, ptr %0, i64 12676
  %5 = load i32, ptr %4, align 4
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BC@ONCCKKPK@Positional?5?$CI?$CFd?$CJ?3?6?$AA@", i32 noundef %5)
  %7 = load i32, ptr %4, align 4
  %8 = icmp sgt i32 %7, 0
  br i1 %8, label %9, label %46

9:                                                ; preds = %3
  %10 = getelementptr inbounds nuw i8, ptr %0, i64 4484
  br label %47

11:                                               ; preds = %1, %43
  %12 = phi i64 [ 0, %1 ], [ %44, %43 ]
  %13 = getelementptr inbounds nuw %struct.OptValue, ptr %0, i64 %12
  %14 = getelementptr inbounds nuw %struct.OptSpec, ptr @schema, i64 %12
  %15 = getelementptr inbounds nuw i8, ptr %14, i64 64
  %16 = load i8, ptr %15, align 8
  %17 = sext i8 %16 to i32
  %18 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0P@BLENMIJJ@?5?5?9?9?$CFs?5?$CI?9?$CFc?$CJ?3?5?$AA@", ptr noundef nonnull %14, i32 noundef %17)
  %19 = load i32, ptr %13, align 8
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %23

21:                                               ; preds = %11
  %22 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0L@CIMKDCEL@?$FLdefault?$FN?5?$AA@")
  br label %23

23:                                               ; preds = %21, %11
  %24 = getelementptr inbounds nuw i8, ptr %14, i64 68
  %25 = load i32, ptr %24, align 4
  switch i32 %25, label %43 [
    i32 0, label %26
    i32 1, label %32
    i32 2, label %35
    i32 3, label %39
  ]

26:                                               ; preds = %23
  %27 = getelementptr inbounds nuw i8, ptr %13, i64 4
  %28 = load i32, ptr %27, align 4
  %29 = icmp eq i32 %28, 0
  %30 = select i1 %29, ptr @"??_C@_05LAPONLG@false?$AA@", ptr @"??_C@_04LOAJBDKD@true?$AA@"
  %31 = tail call i32 @puts(ptr nonnull dereferenceable(1) %30)
  br label %43

32:                                               ; preds = %23
  %33 = getelementptr inbounds nuw i8, ptr %13, i64 8
  %34 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_05LFNECPGD@?$CC?$CFs?$CC?6?$AA@", ptr noundef nonnull %33)
  br label %43

35:                                               ; preds = %23
  %36 = getelementptr inbounds nuw i8, ptr %13, i64 264
  %37 = load i32, ptr %36, align 8
  %38 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_03PMGGPEJJ@?$CFd?6?$AA@", i32 noundef %37)
  br label %43

39:                                               ; preds = %23
  %40 = getelementptr inbounds nuw i8, ptr %13, i64 272
  %41 = load double, ptr %40, align 8
  %42 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_05KOFBGLFC@?$CF?44f?6?$AA@", double noundef %41)
  br label %43

43:                                               ; preds = %23, %39, %35, %32, %26
  %44 = add nuw nsw i64 %12, 1
  %45 = icmp eq i64 %44, 7
  br i1 %45, label %3, label %11, !llvm.loop !13

46:                                               ; preds = %47, %3
  ret void

47:                                               ; preds = %9, %47
  %48 = phi i64 [ 0, %9 ], [ %52, %47 ]
  %49 = getelementptr inbounds nuw [256 x i8], ptr %10, i64 %48
  %50 = trunc nuw nsw i64 %48 to i32
  %51 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0L@CJLEEMNJ@?5?5?$FL?$CFd?$FN?5?$CFs?6?$AA@", i32 noundef %50, ptr noundef nonnull %49)
  %52 = add nuw nsw i64 %48, 1
  %53 = load i32, ptr %4, align 4
  %54 = sext i32 %53 to i64
  %55 = icmp slt i64 %52, %54
  br i1 %55, label %47, label %46, !llvm.loop !14
}

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: read) uwtable
define internal fastcc i32 @result_checksum(ptr noundef nonnull readonly captures(none) %0) unnamed_addr #4 {
  br label %9

2:                                                ; preds = %69
  %3 = getelementptr inbounds nuw i8, ptr %0, i64 12676
  %4 = load i32, ptr %3, align 4
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %6, label %73

6:                                                ; preds = %2
  %7 = getelementptr inbounds nuw i8, ptr %0, i64 4484
  %8 = zext nneg i32 %4 to i64
  br label %75

9:                                                ; preds = %1, %69
  %10 = phi i64 [ 0, %1 ], [ %71, %69 ]
  %11 = phi i32 [ 0, %1 ], [ %70, %69 ]
  %12 = getelementptr inbounds nuw %struct.OptSpec, ptr @schema, i64 %10
  %13 = load i8, ptr %12, align 8
  br label %14

14:                                               ; preds = %9, %14
  %15 = phi i8 [ %22, %14 ], [ %13, %9 ]
  %16 = phi i32 [ %21, %14 ], [ 5381, %9 ]
  %17 = phi ptr [ %19, %14 ], [ %12, %9 ]
  %18 = mul i32 %16, 33
  %19 = getelementptr inbounds nuw i8, ptr %17, i64 1
  %20 = zext i8 %15 to i32
  %21 = xor i32 %18, %20
  %22 = load i8, ptr %19, align 1
  %23 = icmp eq i8 %22, 0
  br i1 %23, label %24, label %14, !llvm.loop !15

24:                                               ; preds = %14
  %25 = getelementptr inbounds nuw %struct.OptValue, ptr %0, i64 %10
  %26 = xor i32 %21, %11
  %27 = load i32, ptr %25, align 8
  %28 = icmp eq i32 %27, 0
  %29 = trunc nuw nsw i64 %10 to i32
  %30 = mul i32 %29, -1640531535
  %31 = add i32 %30, -1640531535
  %32 = select i1 %28, i32 0, i32 %31
  %33 = add i32 %32, %26
  %34 = getelementptr inbounds nuw i8, ptr %12, i64 68
  %35 = load i32, ptr %34, align 4
  switch i32 %35, label %69 [
    i32 0, label %36
    i32 1, label %41
    i32 2, label %58
    i32 3, label %63
  ]

36:                                               ; preds = %24
  %37 = getelementptr inbounds nuw i8, ptr %25, i64 4
  %38 = load i32, ptr %37, align 4
  %39 = shl i32 %38, %29
  %40 = xor i32 %39, %33
  br label %69

41:                                               ; preds = %24
  %42 = getelementptr inbounds nuw i8, ptr %25, i64 8
  %43 = load i8, ptr %42, align 1
  %44 = icmp eq i8 %43, 0
  br i1 %44, label %55, label %45

45:                                               ; preds = %41, %45
  %46 = phi i8 [ %53, %45 ], [ %43, %41 ]
  %47 = phi i32 [ %52, %45 ], [ 5381, %41 ]
  %48 = phi ptr [ %50, %45 ], [ %42, %41 ]
  %49 = mul i32 %47, 33
  %50 = getelementptr inbounds nuw i8, ptr %48, i64 1
  %51 = zext i8 %46 to i32
  %52 = xor i32 %49, %51
  %53 = load i8, ptr %50, align 1
  %54 = icmp eq i8 %53, 0
  br i1 %54, label %55, label %45, !llvm.loop !15

55:                                               ; preds = %45, %41
  %56 = phi i32 [ 5381, %41 ], [ %52, %45 ]
  %57 = xor i32 %56, %33
  br label %69

58:                                               ; preds = %24
  %59 = getelementptr inbounds nuw i8, ptr %25, i64 264
  %60 = load i32, ptr %59, align 8
  %61 = mul i32 %60, 1597334677
  %62 = add i32 %61, %33
  br label %69

63:                                               ; preds = %24
  %64 = getelementptr inbounds nuw i8, ptr %25, i64 272
  %65 = load double, ptr %64, align 8
  %66 = fmul double %65, 1.000000e+03
  %67 = fptoui double %66 to i32
  %68 = xor i32 %33, %67
  br label %69

69:                                               ; preds = %24, %63, %58, %55, %36
  %70 = phi i32 [ %33, %24 ], [ %40, %36 ], [ %57, %55 ], [ %62, %58 ], [ %68, %63 ]
  %71 = add nuw nsw i64 %10, 1
  %72 = icmp eq i64 %71, 7
  br i1 %72, label %2, label %9, !llvm.loop !16

73:                                               ; preds = %91, %2
  %74 = phi i32 [ %70, %2 ], [ %96, %91 ]
  ret i32 %74

75:                                               ; preds = %6, %91
  %76 = phi i64 [ 0, %6 ], [ %93, %91 ]
  %77 = phi i32 [ %70, %6 ], [ %96, %91 ]
  %78 = getelementptr inbounds nuw [256 x i8], ptr %7, i64 %76
  %79 = load i8, ptr %78, align 1
  %80 = icmp eq i8 %79, 0
  br i1 %80, label %91, label %81

81:                                               ; preds = %75, %81
  %82 = phi i8 [ %89, %81 ], [ %79, %75 ]
  %83 = phi i32 [ %88, %81 ], [ 5381, %75 ]
  %84 = phi ptr [ %86, %81 ], [ %78, %75 ]
  %85 = mul i32 %83, 33
  %86 = getelementptr inbounds nuw i8, ptr %84, i64 1
  %87 = zext i8 %82 to i32
  %88 = xor i32 %85, %87
  %89 = load i8, ptr %86, align 1
  %90 = icmp eq i8 %89, 0
  br i1 %90, label %91, label %81, !llvm.loop !15

91:                                               ; preds = %81, %75
  %92 = phi i32 [ 5381, %75 ], [ %88, %81 ]
  %93 = add nuw nsw i64 %76, 1
  %94 = trunc nuw nsw i64 %93 to i32
  %95 = mul i32 %92, %94
  %96 = xor i32 %95, %77
  %97 = icmp eq i64 %93, %8
  br i1 %97, label %73, label %75, !llvm.loop !17
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(ptr captures(none)) #2

; Function Attrs: mustprogress nofree norecurse nounwind willreturn memory(argmem: read) uwtable
define internal fastcc range(i32 -1, 7) i32 @find_long_option(ptr noundef readonly captures(none) %0) unnamed_addr #5 {
  %2 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) @schema, ptr noundef nonnull readonly dereferenceable(1) %0) #15
  %3 = icmp eq i32 %2, 0
  br i1 %3, label %23, label %4

4:                                                ; preds = %1
  %5 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) getelementptr inbounds nuw (i8, ptr @schema, i64 424), ptr noundef nonnull readonly dereferenceable(1) %0) #15
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %23, label %7

7:                                                ; preds = %4
  %8 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) getelementptr inbounds nuw (i8, ptr @schema, i64 848), ptr noundef nonnull readonly dereferenceable(1) %0) #15
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %23, label %10

10:                                               ; preds = %7
  %11 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) getelementptr inbounds nuw (i8, ptr @schema, i64 1272), ptr noundef nonnull readonly dereferenceable(1) %0) #15
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %23, label %13

13:                                               ; preds = %10
  %14 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) getelementptr inbounds nuw (i8, ptr @schema, i64 1696), ptr noundef nonnull readonly dereferenceable(1) %0) #15
  %15 = icmp eq i32 %14, 0
  br i1 %15, label %23, label %16

16:                                               ; preds = %13
  %17 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) getelementptr inbounds nuw (i8, ptr @schema, i64 2120), ptr noundef nonnull readonly dereferenceable(1) %0) #15
  %18 = icmp eq i32 %17, 0
  br i1 %18, label %23, label %19

19:                                               ; preds = %16
  %20 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) getelementptr inbounds nuw (i8, ptr @schema, i64 2544), ptr noundef nonnull readonly dereferenceable(1) %0) #15
  %21 = icmp eq i32 %20, 0
  %22 = select i1 %21, i32 6, i32 -1
  br label %23

23:                                               ; preds = %19, %16, %13, %10, %7, %4, %1
  %24 = phi i32 [ 0, %1 ], [ 5, %16 ], [ 1, %4 ], [ %22, %19 ], [ 2, %7 ], [ 4, %13 ], [ 3, %10 ]
  ret i32 %24
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #6

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #6

declare dso_local i32 @__stdio_common_vsprintf(i64 noundef, ptr noundef, i64 noundef, ptr noundef, ptr noundef, ptr noundef) local_unnamed_addr #3

; Function Attrs: noinline nounwind uwtable
define linkonce_odr dso_local ptr @__local_stdio_printf_options() local_unnamed_addr #7 comdat {
  ret ptr @__local_stdio_printf_options._OptionsStorage
}

declare dso_local i32 @__stdio_common_vfprintf(i64 noundef, ptr noundef, ptr noundef, ptr noundef, ptr noundef) local_unnamed_addr #3

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: read)
declare dso_local ptr @strchr(ptr noundef, i32 noundef) local_unnamed_addr #8

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #9

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare dso_local ptr @strncpy(ptr noalias noundef returned writeonly, ptr noalias noundef readonly captures(none), i64 noundef) local_unnamed_addr #10

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @snprintf(ptr noundef %0, i64 noundef %1, ptr noundef %2, ...) local_unnamed_addr #1 comdat {
  %4 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %4) #15
  call void @llvm.va_start.p0(ptr nonnull %4)
  %5 = load ptr, ptr %4, align 8
  %6 = call ptr @__local_stdio_printf_options()
  %7 = load i64, ptr %6, align 8
  %8 = or i64 %7, 2
  %9 = call i32 @__stdio_common_vsprintf(i64 noundef %8, ptr noundef %0, i64 noundef %1, ptr noundef %2, ptr noundef null, ptr noundef %5) #15
  %10 = call i32 @llvm.smax.i32(i32 %9, i32 -1)
  call void @llvm.va_end.p0(ptr nonnull %4)
  call void @llvm.lifetime.end.p0(ptr nonnull %4) #15
  ret i32 %10
}

; Function Attrs: nounwind uwtable
define internal fastcc range(i32 -1, 1) i32 @set_option(ptr noundef nonnull %0, i32 noundef range(i32 0, -2147483648) %1, ptr noundef %2) unnamed_addr #0 {
  %4 = zext nneg i32 %1 to i64
  %5 = getelementptr inbounds nuw %struct.OptValue, ptr %0, i64 %4
  store i32 1, ptr %5, align 8
  %6 = getelementptr inbounds nuw %struct.OptSpec, ptr @schema, i64 %4
  %7 = getelementptr inbounds nuw i8, ptr %6, i64 68
  %8 = load i32, ptr %7, align 4
  switch i32 %8, label %75 [
    i32 0, label %9
    i32 1, label %11
    i32 2, label %57
    i32 3, label %66
  ]

9:                                                ; preds = %3
  %10 = getelementptr inbounds nuw i8, ptr %5, i64 4
  store i32 1, ptr %10, align 4
  br label %75

11:                                               ; preds = %3
  %12 = icmp eq ptr %2, null
  br i1 %12, label %13, label %17

13:                                               ; preds = %11
  %14 = getelementptr inbounds nuw i8, ptr %0, i64 12684
  %15 = tail call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %14, i64 noundef 256, ptr noundef nonnull @"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@", ptr noundef nonnull %6) #15
  %16 = getelementptr inbounds nuw i8, ptr %0, i64 12680
  store i32 1, ptr %16, align 8
  br label %75

17:                                               ; preds = %11
  %18 = icmp eq i32 %1, 6
  br i1 %18, label %19, label %40

19:                                               ; preds = %17
  %20 = getelementptr inbounds nuw i8, ptr %6, i64 400
  %21 = load i32, ptr %20, align 8
  %22 = getelementptr inbounds nuw i8, ptr %6, i64 336
  %23 = tail call i32 @llvm.smax.i32(i32 %21, i32 1)
  %24 = zext nneg i32 %23 to i64
  br label %25

25:                                               ; preds = %33, %19
  %26 = phi i64 [ 0, %19 ], [ %34, %33 ]
  %27 = getelementptr inbounds nuw ptr, ptr %22, i64 %26
  %28 = load ptr, ptr %27, align 8
  %29 = icmp eq ptr %28, null
  br i1 %29, label %33, label %30

30:                                               ; preds = %25
  %31 = tail call i32 @strcmp(ptr noundef nonnull readonly dereferenceable(1) %28, ptr noundef nonnull readonly dereferenceable(1) %2) #15
  %32 = icmp eq i32 %31, 0
  br i1 %32, label %40, label %33

33:                                               ; preds = %30, %25
  %34 = add nuw nsw i64 %26, 1
  %35 = icmp eq i64 %34, %24
  br i1 %35, label %36, label %25, !llvm.loop !18

36:                                               ; preds = %33
  %37 = getelementptr inbounds nuw i8, ptr %0, i64 12684
  %38 = tail call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %37, i64 noundef 256, ptr noundef nonnull @"??_C@_0BM@JBPENILB@Invalid?5value?5?8?$CFs?8?5for?5?9?9?$CFs?$AA@", ptr noundef nonnull %2, ptr noundef nonnull %6) #15
  %39 = getelementptr inbounds nuw i8, ptr %0, i64 12680
  store i32 1, ptr %39, align 8
  br label %75

40:                                               ; preds = %30, %17
  %41 = getelementptr inbounds nuw i8, ptr %5, i64 8
  %42 = load i8, ptr %2, align 1
  %43 = icmp eq i8 %42, 0
  br i1 %43, label %54, label %44

44:                                               ; preds = %40, %44
  %45 = phi i64 [ %48, %44 ], [ 0, %40 ]
  %46 = phi i8 [ %50, %44 ], [ %42, %40 ]
  %47 = getelementptr inbounds nuw i8, ptr %41, i64 %45
  store i8 %46, ptr %47, align 1
  %48 = add nuw nsw i64 %45, 1
  %49 = getelementptr inbounds nuw i8, ptr %2, i64 %48
  %50 = load i8, ptr %49, align 1
  %51 = icmp ne i8 %50, 0
  %52 = icmp samesign ult i64 %45, 254
  %53 = select i1 %51, i1 %52, i1 false
  br i1 %53, label %44, label %54, !llvm.loop !10

54:                                               ; preds = %44, %40
  %55 = phi i64 [ 0, %40 ], [ %48, %44 ]
  %56 = getelementptr inbounds nuw i8, ptr %41, i64 %55
  store i8 0, ptr %56, align 1
  br label %75

57:                                               ; preds = %3
  %58 = icmp eq ptr %2, null
  br i1 %58, label %59, label %63

59:                                               ; preds = %57
  %60 = getelementptr inbounds nuw i8, ptr %0, i64 12684
  %61 = tail call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %60, i64 noundef 256, ptr noundef nonnull @"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@", ptr noundef nonnull %6) #15
  %62 = getelementptr inbounds nuw i8, ptr %0, i64 12680
  store i32 1, ptr %62, align 8
  br label %75

63:                                               ; preds = %57
  %64 = tail call i32 @atoi(ptr noundef nonnull %2)
  %65 = getelementptr inbounds nuw i8, ptr %5, i64 264
  store i32 %64, ptr %65, align 8
  br label %75

66:                                               ; preds = %3
  %67 = icmp eq ptr %2, null
  br i1 %67, label %68, label %72

68:                                               ; preds = %66
  %69 = getelementptr inbounds nuw i8, ptr %0, i64 12684
  %70 = tail call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef nonnull dereferenceable(1) %69, i64 noundef 256, ptr noundef nonnull @"??_C@_0BN@KJGNHICL@Option?5?9?9?$CFs?5requires?5a?5value?$AA@", ptr noundef nonnull %6) #15
  %71 = getelementptr inbounds nuw i8, ptr %0, i64 12680
  store i32 1, ptr %71, align 8
  br label %75

72:                                               ; preds = %66
  %73 = tail call double @atof(ptr noundef nonnull %2)
  %74 = getelementptr inbounds nuw i8, ptr %5, i64 272
  store double %73, ptr %74, align 8
  br label %75

75:                                               ; preds = %9, %54, %63, %72, %3, %68, %59, %36, %13
  %76 = phi i32 [ -1, %68 ], [ -1, %36 ], [ -1, %13 ], [ -1, %59 ], [ 0, %3 ], [ 0, %72 ], [ 0, %63 ], [ 0, %54 ], [ 0, %9 ]
  ret i32 %76
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #11

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: read)
declare dso_local i32 @strcmp(ptr noundef captures(none), ptr noundef captures(none)) local_unnamed_addr #8

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr noundef captures(none)) local_unnamed_addr #12

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(read)
declare dso_local double @atof(ptr noundef captures(none)) local_unnamed_addr #12

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #13

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #13

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smin.i32(i32, i32) #14

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #14

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { inlinehint nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree norecurse nosync nounwind memory(argmem: read) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress nofree norecurse nounwind willreturn memory(argmem: read) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress nocallback nofree nosync nounwind willreturn }
attributes #7 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: read) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #9 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #10 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #11 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #12 = { mustprogress nocallback nofree nounwind willreturn memory(read) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #13 = { nofree nounwind }
attributes #14 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #15 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 22.1.2 (https://github.com/llvm/llvm-project 1ab49a973e210e97d61e5db6557180dcb92c3e98)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "argparse.c", directory: "C:\\Users\\Treverm\\Documents\\Claude Code\\llvm-obfuscator\\eval\\benchmarks")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 2}
!4 = !{i32 8, !"PIC Level", i32 2}
!5 = !{i32 7, !"uwtable", i32 2}
!6 = !{i32 1, !"MaxTLSAlign", i32 65536}
!7 = !{!"clang version 22.1.2 (https://github.com/llvm/llvm-project 1ab49a973e210e97d61e5db6557180dcb92c3e98)"}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.mustprogress"}
!10 = distinct !{!10, !9}
!11 = distinct !{!11, !9}
!12 = distinct !{!12, !9}
!13 = distinct !{!13, !9}
!14 = distinct !{!14, !9}
!15 = distinct !{!15, !9}
!16 = distinct !{!16, !9}
!17 = distinct !{!17, !9}
!18 = distinct !{!18, !9}
