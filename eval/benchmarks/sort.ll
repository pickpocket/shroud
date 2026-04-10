; ModuleID = 'sort.c'
source_filename = "sort.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.44.35224"

$fprintf = comdat any

$printf = comdat any

$__local_stdio_printf_options = comdat any

$"??_C@_0N@BBBFGDMM@Usage?3?5?$CFs?5N?6?$AA@" = comdat any

$"??_C@_0CC@JOIJGLPO@N?5must?5be?5between?51?5and?510000000@" = comdat any

$"??_C@_0P@LEGHBOFO@malloc?5failed?6?$AA@" = comdat any

$"??_C@_0BP@EEOBNLFC@?$DN?$DN?$DN?5Sort?5Benchmark?5?$CIN?$DN?$CFd?$CJ?5?$DN?$DN?$DN?6?$AA@" = comdat any

$"??_C@_0CD@HJOPLMNI@FAIL?3?5quicksort?5output?5not?5sorte@" = comdat any

$"??_C@_0CD@FPOMHOKN@FAIL?3?5mergesort?5output?5not?5sorte@" = comdat any

$"??_C@_0DK@MDPPKBMN@FAIL?3?5quicksort?5and?5mergesort?5pr@" = comdat any

$"??_C@_09GJBCJAEC@quicksort?$AA@" = comdat any

$"??_C@_09BHNENLEL@mergesort?$AA@" = comdat any

$"??_C@_0BA@BJCMKHMP@Checksum?3?5?$CF08x?6?$AA@" = comdat any

$"??_C@_0BA@GNOJLAFJ@Bitmix?3?5?5?5?$CF08x?6?$AA@" = comdat any

$"??_C@_0BA@CLDAKMOE@?5?5elements?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_0BE@NLKHHPEI@?5?5min?3?5?$CFd?0?5max?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_0N@JFJKIOIA@?5?5sum?3?5?$CFlld?6?$AA@" = comdat any

$"??_C@_0BK@JOEFIJDD@mergesort?3?5malloc?5failed?6?$AA@" = comdat any

$"??_C@_0BG@DDAOPMNN@malloc?5failed?5for?5?$CFs?6?$AA@" = comdat any

$"??_C@_0CI@BFKHFIIN@FAIL?3?5?$CFs?5did?5not?5produce?5sorted?5@" = comdat any

$"??_C@_0BP@NHICFENK@FAIL?3?5?$CFs?5element?5sum?5mismatch?6?$AA@" = comdat any

@"??_C@_0N@BBBFGDMM@Usage?3?5?$CFs?5N?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [13 x i8] c"Usage: %s N\0A\00", comdat, align 1
@"??_C@_0CC@JOIJGLPO@N?5must?5be?5between?51?5and?510000000@" = linkonce_odr dso_local unnamed_addr constant [34 x i8] c"N must be between 1 and 10000000\0A\00", comdat, align 1
@"??_C@_0P@LEGHBOFO@malloc?5failed?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"malloc failed\0A\00", comdat, align 1
@"??_C@_0BP@EEOBNLFC@?$DN?$DN?$DN?5Sort?5Benchmark?5?$CIN?$DN?$CFd?$CJ?5?$DN?$DN?$DN?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [31 x i8] c"=== Sort Benchmark (N=%d) ===\0A\00", comdat, align 1
@"??_C@_0CD@HJOPLMNI@FAIL?3?5quicksort?5output?5not?5sorte@" = linkonce_odr dso_local unnamed_addr constant [35 x i8] c"FAIL: quicksort output not sorted\0A\00", comdat, align 1
@"??_C@_0CD@FPOMHOKN@FAIL?3?5mergesort?5output?5not?5sorte@" = linkonce_odr dso_local unnamed_addr constant [35 x i8] c"FAIL: mergesort output not sorted\0A\00", comdat, align 1
@"??_C@_0DK@MDPPKBMN@FAIL?3?5quicksort?5and?5mergesort?5pr@" = linkonce_odr dso_local unnamed_addr constant [58 x i8] c"FAIL: quicksort and mergesort produced different results\0A\00", comdat, align 1
@"??_C@_09GJBCJAEC@quicksort?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"quicksort\00", comdat, align 1
@"??_C@_09BHNENLEL@mergesort?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"mergesort\00", comdat, align 1
@"??_C@_0BA@BJCMKHMP@Checksum?3?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [16 x i8] c"Checksum: %08x\0A\00", comdat, align 1
@"??_C@_0BA@GNOJLAFJ@Bitmix?3?5?5?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [16 x i8] c"Bitmix:   %08x\0A\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@"??_C@_0BA@CLDAKMOE@?5?5elements?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [16 x i8] c"  elements: %d\0A\00", comdat, align 1
@"??_C@_0BE@NLKHHPEI@?5?5min?3?5?$CFd?0?5max?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [20 x i8] c"  min: %d, max: %d\0A\00", comdat, align 1
@"??_C@_0N@JFJKIOIA@?5?5sum?3?5?$CFlld?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [13 x i8] c"  sum: %lld\0A\00", comdat, align 1
@"??_C@_0BK@JOEFIJDD@mergesort?3?5malloc?5failed?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [26 x i8] c"mergesort: malloc failed\0A\00", comdat, align 1
@"??_C@_0BG@DDAOPMNN@malloc?5failed?5for?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [22 x i8] c"malloc failed for %s\0A\00", comdat, align 1
@"??_C@_0CI@BFKHFIIN@FAIL?3?5?$CFs?5did?5not?5produce?5sorted?5@" = linkonce_odr dso_local unnamed_addr constant [40 x i8] c"FAIL: %s did not produce sorted output\0A\00", comdat, align 1
@"??_C@_0BP@NHICFENK@FAIL?3?5?$CFs?5element?5sum?5mismatch?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [31 x i8] c"FAIL: %s element sum mismatch\0A\00", comdat, align 1
@str = private unnamed_addr constant [19 x i8] c"Input array stats:\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Sorted array stats:\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"RESULT: PASS\00", align 1
@str.3 = private unnamed_addr constant [13 x i8] c"RESULT: FAIL\00", align 1

; Function Attrs: nounwind uwtable
define dso_local range(i32 0, 2) i32 @main(i32 noundef %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #0 {
  %3 = icmp eq i32 %0, 2
  br i1 %3, label %8, label %4

4:                                                ; preds = %2
  %5 = load ptr, ptr %1, align 8
  %6 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %7 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef %6, ptr noundef nonnull @"??_C@_0N@BBBFGDMM@Usage?3?5?$CFs?5N?6?$AA@", ptr noundef %5) #16
  br label %339

8:                                                ; preds = %2
  %9 = getelementptr inbounds nuw i8, ptr %1, i64 8
  %10 = load ptr, ptr %9, align 8
  %11 = tail call i32 @atoi(ptr noundef %10)
  %12 = add i32 %11, -10000001
  %13 = icmp ult i32 %12, -10000000
  br i1 %13, label %14, label %17

14:                                               ; preds = %8
  %15 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %16 = tail call i64 @fwrite(ptr nonnull @"??_C@_0CC@JOIJGLPO@N?5must?5be?5between?51?5and?510000000@", i64 33, i64 1, ptr %15)
  br label %339

17:                                               ; preds = %8
  %18 = shl nuw nsw i32 %11, 2
  %19 = zext nneg i32 %18 to i64
  %20 = tail call noalias ptr @malloc(i64 noundef %19) #17
  %21 = icmp eq ptr %20, null
  br i1 %21, label %22, label %25

22:                                               ; preds = %17
  %23 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %24 = tail call i64 @fwrite(ptr nonnull @"??_C@_0P@LEGHBOFO@malloc?5failed?6?$AA@", i64 14, i64 1, ptr %23)
  br label %339

25:                                               ; preds = %17
  %26 = zext nneg i32 %11 to i64
  br label %27

27:                                               ; preds = %27, %25
  %28 = phi i64 [ 0, %25 ], [ %39, %27 ]
  %29 = phi i32 [ 42, %25 ], [ %35, %27 ]
  %30 = shl i32 %29, 13
  %31 = xor i32 %30, %29
  %32 = lshr i32 %31, 17
  %33 = xor i32 %32, %31
  %34 = shl i32 %33, 5
  %35 = xor i32 %34, %33
  %36 = urem i32 %35, 1000000
  %37 = add nsw i32 %36, -500000
  %38 = getelementptr inbounds nuw i32, ptr %20, i64 %28
  store i32 %37, ptr %38, align 4
  %39 = add nuw nsw i64 %28, 1
  %40 = icmp eq i64 %39, %26
  br i1 %40, label %41, label %27, !llvm.loop !8

41:                                               ; preds = %27
  %42 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BP@EEOBNLFC@?$DN?$DN?$DN?5Sort?5Benchmark?5?$CIN?$DN?$CFd?$CJ?5?$DN?$DN?$DN?6?$AA@", i32 noundef %11)
  %43 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %44 = load i32, ptr %20, align 4
  %45 = icmp ult i32 %11, 4
  br i1 %45, label %80, label %46

46:                                               ; preds = %41
  %47 = and i64 %26, 16777212
  %48 = insertelement <2 x i32> poison, i32 %44, i64 0
  %49 = shufflevector <2 x i32> %48, <2 x i32> poison, <2 x i32> zeroinitializer
  br label %50

50:                                               ; preds = %50, %46
  %51 = phi i64 [ 0, %46 ], [ %70, %50 ]
  %52 = phi <2 x i32> [ %49, %46 ], [ %68, %50 ]
  %53 = phi <2 x i32> [ %49, %46 ], [ %69, %50 ]
  %54 = phi <2 x i32> [ %49, %46 ], [ %66, %50 ]
  %55 = phi <2 x i32> [ %49, %46 ], [ %67, %50 ]
  %56 = phi <2 x i64> [ zeroinitializer, %46 ], [ %64, %50 ]
  %57 = phi <2 x i64> [ zeroinitializer, %46 ], [ %65, %50 ]
  %58 = getelementptr inbounds nuw i32, ptr %20, i64 %51
  %59 = getelementptr inbounds nuw i8, ptr %58, i64 8
  %60 = load <2 x i32>, ptr %58, align 4
  %61 = load <2 x i32>, ptr %59, align 4
  %62 = sext <2 x i32> %60 to <2 x i64>
  %63 = sext <2 x i32> %61 to <2 x i64>
  %64 = add <2 x i64> %56, %62
  %65 = add <2 x i64> %57, %63
  %66 = tail call <2 x i32> @llvm.smin.v2i32(<2 x i32> %60, <2 x i32> %54)
  %67 = tail call <2 x i32> @llvm.smin.v2i32(<2 x i32> %61, <2 x i32> %55)
  %68 = tail call <2 x i32> @llvm.smax.v2i32(<2 x i32> %60, <2 x i32> %52)
  %69 = tail call <2 x i32> @llvm.smax.v2i32(<2 x i32> %61, <2 x i32> %53)
  %70 = add nuw i64 %51, 4
  %71 = icmp eq i64 %70, %47
  br i1 %71, label %72, label %50, !llvm.loop !10

72:                                               ; preds = %50
  %73 = tail call <2 x i32> @llvm.smax.v2i32(<2 x i32> %68, <2 x i32> %69)
  %74 = tail call i32 @llvm.vector.reduce.smax.v2i32(<2 x i32> %73)
  %75 = tail call <2 x i32> @llvm.smin.v2i32(<2 x i32> %66, <2 x i32> %67)
  %76 = tail call i32 @llvm.vector.reduce.smin.v2i32(<2 x i32> %75)
  %77 = add <2 x i64> %65, %64
  %78 = tail call i64 @llvm.vector.reduce.add.v2i64(<2 x i64> %77)
  %79 = icmp eq i64 %47, %26
  br i1 %79, label %98, label %80

80:                                               ; preds = %41, %72
  %81 = phi i64 [ 0, %41 ], [ %47, %72 ]
  %82 = phi i32 [ %44, %41 ], [ %74, %72 ]
  %83 = phi i32 [ %44, %41 ], [ %76, %72 ]
  %84 = phi i64 [ 0, %41 ], [ %78, %72 ]
  br label %85

85:                                               ; preds = %80, %85
  %86 = phi i64 [ %96, %85 ], [ %81, %80 ]
  %87 = phi i32 [ %95, %85 ], [ %82, %80 ]
  %88 = phi i32 [ %94, %85 ], [ %83, %80 ]
  %89 = phi i64 [ %93, %85 ], [ %84, %80 ]
  %90 = getelementptr inbounds nuw i32, ptr %20, i64 %86
  %91 = load i32, ptr %90, align 4
  %92 = sext i32 %91 to i64
  %93 = add nsw i64 %89, %92
  %94 = tail call i32 @llvm.smin.i32(i32 %91, i32 %88)
  %95 = tail call i32 @llvm.smax.i32(i32 %91, i32 %87)
  %96 = add nuw nsw i64 %86, 1
  %97 = icmp eq i64 %96, %26
  br i1 %97, label %98, label %85, !llvm.loop !13

98:                                               ; preds = %85, %72
  %99 = phi i64 [ %78, %72 ], [ %93, %85 ]
  %100 = phi i32 [ %76, %72 ], [ %94, %85 ]
  %101 = phi i32 [ %74, %72 ], [ %95, %85 ]
  %102 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BA@CLDAKMOE@?5?5elements?3?5?$CFd?6?$AA@", i32 noundef range(i32 1, 10000001) %11)
  %103 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BE@NLKHHPEI@?5?5min?3?5?$CFd?0?5max?3?5?$CFd?6?$AA@", i32 noundef %100, i32 noundef %101)
  %104 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0N@JFJKIOIA@?5?5sum?3?5?$CFlld?6?$AA@", i64 noundef %99)
  %105 = tail call noalias ptr @malloc(i64 noundef %19) #17
  %106 = icmp eq ptr %105, null
  br i1 %106, label %107, label %108

107:                                              ; preds = %98
  tail call void @free(ptr noundef nonnull %20)
  br label %339

108:                                              ; preds = %98
  tail call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 4 dereferenceable(1) %105, ptr noundef nonnull readonly align 4 dereferenceable(1) %20, i64 %19, i1 false)
  %109 = icmp samesign ugt i32 %11, 1
  br i1 %109, label %110, label %112

110:                                              ; preds = %108
  %111 = add nsw i32 %11, -1
  tail call fastcc void @quicksort_impl(ptr noundef nonnull %105, i32 noundef 0, i32 noundef %111)
  br label %112

112:                                              ; preds = %108, %110
  %113 = tail call noalias ptr @malloc(i64 noundef %19) #17
  %114 = icmp eq ptr %113, null
  br i1 %114, label %115, label %116

115:                                              ; preds = %112
  tail call void @free(ptr noundef nonnull %20)
  tail call void @free(ptr noundef nonnull %105)
  br label %339

116:                                              ; preds = %112
  tail call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 4 dereferenceable(1) %113, ptr noundef nonnull readonly align 4 dereferenceable(1) %20, i64 %19, i1 false)
  tail call void @mergesort(ptr noundef nonnull %113, i32 noundef %11)
  br i1 %109, label %120, label %145

117:                                              ; preds = %120
  %118 = add nuw nsw i64 %121, 1
  %119 = icmp eq i64 %118, %26
  br i1 %119, label %130, label %120, !llvm.loop !14

120:                                              ; preds = %116, %117
  %121 = phi i64 [ %118, %117 ], [ 1, %116 ]
  %122 = getelementptr inbounds nuw i32, ptr %105, i64 %121
  %123 = load i32, ptr %122, align 4
  %124 = getelementptr i8, ptr %122, i64 -4
  %125 = load i32, ptr %124, align 4
  %126 = icmp slt i32 %123, %125
  br i1 %126, label %127, label %117

127:                                              ; preds = %120
  %128 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %129 = tail call i64 @fwrite(ptr nonnull @"??_C@_0CD@HJOPLMNI@FAIL?3?5quicksort?5output?5not?5sorte@", i64 34, i64 1, ptr %128)
  br label %130

130:                                              ; preds = %117, %127
  %131 = phi i32 [ 1, %127 ], [ 0, %117 ]
  br label %135

132:                                              ; preds = %135
  %133 = add nuw nsw i64 %136, 1
  %134 = icmp eq i64 %133, %26
  br i1 %134, label %145, label %135, !llvm.loop !14

135:                                              ; preds = %130, %132
  %136 = phi i64 [ %133, %132 ], [ 1, %130 ]
  %137 = getelementptr inbounds nuw i32, ptr %113, i64 %136
  %138 = load i32, ptr %137, align 4
  %139 = getelementptr i8, ptr %137, i64 -4
  %140 = load i32, ptr %139, align 4
  %141 = icmp slt i32 %138, %140
  br i1 %141, label %142, label %132

142:                                              ; preds = %135
  %143 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %144 = tail call i64 @fwrite(ptr nonnull @"??_C@_0CD@FPOMHOKN@FAIL?3?5mergesort?5output?5not?5sorte@", i64 34, i64 1, ptr %143)
  br label %145

145:                                              ; preds = %132, %116, %142
  %146 = phi i32 [ 1, %142 ], [ 0, %116 ], [ %131, %132 ]
  br label %150

147:                                              ; preds = %150
  %148 = add nuw nsw i64 %151, 1
  %149 = icmp eq i64 %148, %26
  br i1 %149, label %160, label %150, !llvm.loop !15

150:                                              ; preds = %147, %145
  %151 = phi i64 [ 0, %145 ], [ %148, %147 ]
  %152 = getelementptr inbounds nuw i32, ptr %105, i64 %151
  %153 = load i32, ptr %152, align 4
  %154 = getelementptr inbounds nuw i32, ptr %113, i64 %151
  %155 = load i32, ptr %154, align 4
  %156 = icmp eq i32 %153, %155
  br i1 %156, label %147, label %157

157:                                              ; preds = %150
  %158 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %159 = tail call i64 @fwrite(ptr nonnull @"??_C@_0DK@MDPPKBMN@FAIL?3?5quicksort?5and?5mergesort?5pr@", i64 57, i64 1, ptr %158)
  br label %160

160:                                              ; preds = %147, %157
  %161 = phi i32 [ 1, %157 ], [ %146, %147 ]
  %162 = tail call fastcc i32 @verify_sort(ptr noundef %20, i32 noundef %11, ptr noundef nonnull @quicksort, ptr noundef nonnull @"??_C@_09GJBCJAEC@quicksort?$AA@")
  %163 = tail call fastcc i32 @verify_sort(ptr noundef %20, i32 noundef %11, ptr noundef nonnull @mergesort, ptr noundef nonnull @"??_C@_09BHNENLEL@mergesort?$AA@")
  %164 = and i64 %26, 1
  %165 = icmp eq i32 %11, 1
  br i1 %165, label %194, label %166

166:                                              ; preds = %160
  %167 = and i64 %26, 16777214
  br label %168

168:                                              ; preds = %168, %166
  %169 = phi i64 [ 0, %166 ], [ %189, %168 ]
  %170 = phi i32 [ 0, %166 ], [ %188, %168 ]
  %171 = phi i64 [ 0, %166 ], [ %190, %168 ]
  %172 = getelementptr inbounds nuw i32, ptr %105, i64 %169
  %173 = load i32, ptr %172, align 4
  %174 = trunc nuw nsw i64 %169 to i32
  %175 = xor i32 %173, %174
  %176 = tail call i32 @llvm.fshl.i32(i32 %175, i32 %175, i32 7)
  %177 = xor i32 %176, %170
  %178 = mul i32 %173, -1640531535
  %179 = add i32 %177, %178
  %180 = or disjoint i64 %169, 1
  %181 = getelementptr inbounds nuw i32, ptr %105, i64 %180
  %182 = load i32, ptr %181, align 4
  %183 = trunc nuw nsw i64 %180 to i32
  %184 = xor i32 %182, %183
  %185 = tail call i32 @llvm.fshl.i32(i32 %184, i32 %184, i32 7)
  %186 = xor i32 %185, %179
  %187 = mul i32 %182, -1640531535
  %188 = add i32 %186, %187
  %189 = add nuw nsw i64 %169, 2
  %190 = add i64 %171, 2
  %191 = icmp eq i64 %190, %167
  br i1 %191, label %192, label %168, !llvm.loop !16

192:                                              ; preds = %168
  %193 = icmp eq i64 %164, 0
  br i1 %193, label %206, label %194

194:                                              ; preds = %192, %160
  %195 = phi i64 [ 0, %160 ], [ %189, %192 ]
  %196 = phi i32 [ 0, %160 ], [ %188, %192 ]
  %197 = icmp ne i64 %164, 0
  tail call void @llvm.assume(i1 %197)
  %198 = getelementptr inbounds nuw i32, ptr %105, i64 %195
  %199 = load i32, ptr %198, align 4
  %200 = trunc nuw nsw i64 %195 to i32
  %201 = xor i32 %199, %200
  %202 = tail call i32 @llvm.fshl.i32(i32 %201, i32 %201, i32 7)
  %203 = xor i32 %202, %196
  %204 = mul i32 %199, -1640531535
  %205 = add i32 %203, %204
  br label %206

206:                                              ; preds = %192, %194
  %207 = phi i32 [ %188, %192 ], [ %205, %194 ]
  %208 = icmp ult i32 %11, 8
  br i1 %208, label %239, label %209

209:                                              ; preds = %206
  %210 = and i64 %26, 16777208
  br label %211

211:                                              ; preds = %211, %209
  %212 = phi i64 [ 0, %209 ], [ %229, %211 ]
  %213 = phi <4 x i32> [ zeroinitializer, %209 ], [ %223, %211 ]
  %214 = phi <4 x i32> [ zeroinitializer, %209 ], [ %224, %211 ]
  %215 = phi <4 x i32> [ splat (i32 -1), %209 ], [ %225, %211 ]
  %216 = phi <4 x i32> [ splat (i32 -1), %209 ], [ %226, %211 ]
  %217 = phi <4 x i32> [ zeroinitializer, %209 ], [ %227, %211 ]
  %218 = phi <4 x i32> [ zeroinitializer, %209 ], [ %228, %211 ]
  %219 = getelementptr inbounds nuw i32, ptr %105, i64 %212
  %220 = getelementptr inbounds nuw i8, ptr %219, i64 16
  %221 = load <4 x i32>, ptr %219, align 4
  %222 = load <4 x i32>, ptr %220, align 4
  %223 = or <4 x i32> %221, %213
  %224 = or <4 x i32> %222, %214
  %225 = and <4 x i32> %221, %215
  %226 = and <4 x i32> %222, %216
  %227 = xor <4 x i32> %221, %217
  %228 = xor <4 x i32> %222, %218
  %229 = add nuw i64 %212, 8
  %230 = icmp eq i64 %229, %210
  br i1 %230, label %231, label %211, !llvm.loop !17

231:                                              ; preds = %211
  %232 = or <4 x i32> %224, %223
  %233 = tail call i32 @llvm.vector.reduce.or.v4i32(<4 x i32> %232)
  %234 = and <4 x i32> %226, %225
  %235 = tail call i32 @llvm.vector.reduce.and.v4i32(<4 x i32> %234)
  %236 = xor <4 x i32> %228, %227
  %237 = tail call i32 @llvm.vector.reduce.xor.v4i32(<4 x i32> %236)
  %238 = icmp eq i64 %210, %26
  br i1 %238, label %256, label %239

239:                                              ; preds = %206, %231
  %240 = phi i64 [ 0, %206 ], [ %210, %231 ]
  %241 = phi i32 [ 0, %206 ], [ %233, %231 ]
  %242 = phi i32 [ -1, %206 ], [ %235, %231 ]
  %243 = phi i32 [ 0, %206 ], [ %237, %231 ]
  br label %244

244:                                              ; preds = %239, %244
  %245 = phi i64 [ %254, %244 ], [ %240, %239 ]
  %246 = phi i32 [ %251, %244 ], [ %241, %239 ]
  %247 = phi i32 [ %252, %244 ], [ %242, %239 ]
  %248 = phi i32 [ %253, %244 ], [ %243, %239 ]
  %249 = getelementptr inbounds nuw i32, ptr %105, i64 %245
  %250 = load i32, ptr %249, align 4
  %251 = or i32 %250, %246
  %252 = and i32 %250, %247
  %253 = xor i32 %250, %248
  %254 = add nuw nsw i64 %245, 1
  %255 = icmp eq i64 %254, %26
  br i1 %255, label %256, label %244, !llvm.loop !18

256:                                              ; preds = %244, %231
  %257 = phi i32 [ %233, %231 ], [ %251, %244 ]
  %258 = phi i32 [ %235, %231 ], [ %252, %244 ]
  %259 = phi i32 [ %237, %231 ], [ %253, %244 ]
  %260 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  %261 = load i32, ptr %105, align 4
  %262 = icmp ult i32 %11, 4
  br i1 %262, label %297, label %263

263:                                              ; preds = %256
  %264 = and i64 %26, 16777212
  %265 = insertelement <2 x i32> poison, i32 %261, i64 0
  %266 = shufflevector <2 x i32> %265, <2 x i32> poison, <2 x i32> zeroinitializer
  br label %267

267:                                              ; preds = %267, %263
  %268 = phi i64 [ 0, %263 ], [ %287, %267 ]
  %269 = phi <2 x i32> [ %266, %263 ], [ %285, %267 ]
  %270 = phi <2 x i32> [ %266, %263 ], [ %286, %267 ]
  %271 = phi <2 x i32> [ %266, %263 ], [ %283, %267 ]
  %272 = phi <2 x i32> [ %266, %263 ], [ %284, %267 ]
  %273 = phi <2 x i64> [ zeroinitializer, %263 ], [ %281, %267 ]
  %274 = phi <2 x i64> [ zeroinitializer, %263 ], [ %282, %267 ]
  %275 = getelementptr inbounds nuw i32, ptr %105, i64 %268
  %276 = getelementptr inbounds nuw i8, ptr %275, i64 8
  %277 = load <2 x i32>, ptr %275, align 4
  %278 = load <2 x i32>, ptr %276, align 4
  %279 = sext <2 x i32> %277 to <2 x i64>
  %280 = sext <2 x i32> %278 to <2 x i64>
  %281 = add <2 x i64> %273, %279
  %282 = add <2 x i64> %274, %280
  %283 = tail call <2 x i32> @llvm.smin.v2i32(<2 x i32> %277, <2 x i32> %271)
  %284 = tail call <2 x i32> @llvm.smin.v2i32(<2 x i32> %278, <2 x i32> %272)
  %285 = tail call <2 x i32> @llvm.smax.v2i32(<2 x i32> %277, <2 x i32> %269)
  %286 = tail call <2 x i32> @llvm.smax.v2i32(<2 x i32> %278, <2 x i32> %270)
  %287 = add nuw i64 %268, 4
  %288 = icmp eq i64 %287, %264
  br i1 %288, label %289, label %267, !llvm.loop !19

289:                                              ; preds = %267
  %290 = tail call <2 x i32> @llvm.smax.v2i32(<2 x i32> %285, <2 x i32> %286)
  %291 = tail call i32 @llvm.vector.reduce.smax.v2i32(<2 x i32> %290)
  %292 = tail call <2 x i32> @llvm.smin.v2i32(<2 x i32> %283, <2 x i32> %284)
  %293 = tail call i32 @llvm.vector.reduce.smin.v2i32(<2 x i32> %292)
  %294 = add <2 x i64> %282, %281
  %295 = tail call i64 @llvm.vector.reduce.add.v2i64(<2 x i64> %294)
  %296 = icmp eq i64 %264, %26
  br i1 %296, label %315, label %297

297:                                              ; preds = %256, %289
  %298 = phi i64 [ 0, %256 ], [ %264, %289 ]
  %299 = phi i32 [ %261, %256 ], [ %291, %289 ]
  %300 = phi i32 [ %261, %256 ], [ %293, %289 ]
  %301 = phi i64 [ 0, %256 ], [ %295, %289 ]
  br label %302

302:                                              ; preds = %297, %302
  %303 = phi i64 [ %313, %302 ], [ %298, %297 ]
  %304 = phi i32 [ %312, %302 ], [ %299, %297 ]
  %305 = phi i32 [ %311, %302 ], [ %300, %297 ]
  %306 = phi i64 [ %310, %302 ], [ %301, %297 ]
  %307 = getelementptr inbounds nuw i32, ptr %105, i64 %303
  %308 = load i32, ptr %307, align 4
  %309 = sext i32 %308 to i64
  %310 = add nsw i64 %306, %309
  %311 = tail call i32 @llvm.smin.i32(i32 %308, i32 %305)
  %312 = tail call i32 @llvm.smax.i32(i32 %308, i32 %304)
  %313 = add nuw nsw i64 %303, 1
  %314 = icmp eq i64 %313, %26
  br i1 %314, label %315, label %302, !llvm.loop !20

315:                                              ; preds = %302, %289
  %316 = phi i64 [ %295, %289 ], [ %310, %302 ]
  %317 = phi i32 [ %293, %289 ], [ %311, %302 ]
  %318 = phi i32 [ %291, %289 ], [ %312, %302 ]
  %319 = icmp eq i32 %162, 0
  %320 = icmp eq i32 %163, 0
  %321 = select i1 %320, i1 %319, i1 false
  %322 = select i1 %321, i32 %161, i32 1
  %323 = tail call i32 @llvm.fshl.i32(i32 %258, i32 %258, i32 16)
  %324 = xor i32 %323, %257
  %325 = add i32 %324, %259
  %326 = lshr i32 %325, 11
  %327 = xor i32 %326, %325
  %328 = mul i32 %327, -1640531535
  %329 = lshr i32 %328, 13
  %330 = xor i32 %329, %328
  %331 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BA@CLDAKMOE@?5?5elements?3?5?$CFd?6?$AA@", i32 noundef range(i32 1, 10000001) %11)
  %332 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BE@NLKHHPEI@?5?5min?3?5?$CFd?0?5max?3?5?$CFd?6?$AA@", i32 noundef %317, i32 noundef %318)
  %333 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0N@JFJKIOIA@?5?5sum?3?5?$CFlld?6?$AA@", i64 noundef %316)
  %334 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BA@BJCMKHMP@Checksum?3?5?$CF08x?6?$AA@", i32 noundef %207)
  %335 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BA@GNOJLAFJ@Bitmix?3?5?5?5?$CF08x?6?$AA@", i32 noundef %330)
  %336 = icmp eq i32 %322, 0
  %337 = select i1 %336, ptr @str.2, ptr @str.3
  %338 = tail call i32 @puts(ptr nonnull dereferenceable(1) %337)
  tail call void @free(ptr noundef nonnull %20)
  tail call void @free(ptr noundef nonnull %105)
  tail call void @free(ptr noundef nonnull %113)
  br label %339

339:                                              ; preds = %14, %107, %315, %115, %22, %4
  %340 = phi i32 [ 1, %4 ], [ 1, %14 ], [ 1, %22 ], [ 1, %107 ], [ %322, %315 ], [ 1, %115 ]
  ret i32 %340
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @fprintf(ptr noundef %0, ptr noundef %1, ...) local_unnamed_addr #1 comdat {
  %3 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %3) #16
  call void @llvm.va_start.p0(ptr nonnull %3)
  %4 = load ptr, ptr %3, align 8
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %0, ptr noundef %1, ptr noundef null, ptr noundef %4) #16
  call void @llvm.va_end.p0(ptr nonnull %3)
  call void @llvm.lifetime.end.p0(ptr nonnull %3) #16
  ret i32 %7
}

declare dso_local ptr @__acrt_iob_func(i32 noundef) local_unnamed_addr #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #3

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr noundef captures(none)) local_unnamed_addr #4

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite)
declare dso_local noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #5

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @printf(ptr noundef %0, ...) local_unnamed_addr #1 comdat {
  %2 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %2) #16
  call void @llvm.va_start.p0(ptr nonnull %2)
  %3 = load ptr, ptr %2, align 8
  %4 = call ptr @__acrt_iob_func(i32 noundef 1) #16
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %4, ptr noundef %0, ptr noundef null, ptr noundef %3) #16
  call void @llvm.va_end.p0(ptr nonnull %2)
  call void @llvm.lifetime.end.p0(ptr nonnull %2) #16
  ret i32 %7
}

; Function Attrs: mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite)
declare dso_local void @free(ptr allocptr noundef captures(none)) local_unnamed_addr #6

; Function Attrs: nofree nosync nounwind memory(argmem: readwrite) uwtable
define internal void @quicksort(ptr noundef %0, i32 noundef %1) #7 {
  %3 = icmp sgt i32 %1, 1
  br i1 %3, label %4, label %6

4:                                                ; preds = %2
  %5 = add nsw i32 %1, -1
  tail call fastcc void @quicksort_impl(ptr noundef %0, i32 noundef 0, i32 noundef %5)
  br label %6

6:                                                ; preds = %4, %2
  ret void
}

; Function Attrs: nounwind uwtable
define internal void @mergesort(ptr noundef %0, i32 noundef %1) #0 {
  %3 = icmp slt i32 %1, 2
  br i1 %3, label %14, label %4

4:                                                ; preds = %2
  %5 = zext nneg i32 %1 to i64
  %6 = shl nuw nsw i64 %5, 2
  %7 = tail call noalias ptr @malloc(i64 noundef %6) #17
  %8 = icmp eq ptr %7, null
  br i1 %8, label %9, label %12

9:                                                ; preds = %4
  %10 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %11 = tail call i64 @fwrite(ptr nonnull @"??_C@_0BK@JOEFIJDD@mergesort?3?5malloc?5failed?6?$AA@", i64 25, i64 1, ptr %10)
  tail call void @exit(i32 noundef 1) #18
  unreachable

12:                                               ; preds = %4
  %13 = add nsw i32 %1, -1
  tail call fastcc void @mergesort_impl(ptr noundef %0, ptr noundef %7, i32 noundef 0, i32 noundef %13)
  tail call void @free(ptr noundef nonnull %7)
  br label %14

14:                                               ; preds = %2, %12
  ret void
}

; Function Attrs: nounwind uwtable
define internal fastcc range(i32 0, 2) i32 @verify_sort(ptr noundef nonnull readonly captures(none) %0, i32 noundef range(i32 1, 10000001) %1, ptr noundef readonly captures(none) %2, ptr noundef %3) unnamed_addr #0 {
  %5 = shl nuw nsw i32 %1, 2
  %6 = zext nneg i32 %5 to i64
  %7 = tail call noalias ptr @malloc(i64 noundef %6) #17
  %8 = icmp eq ptr %7, null
  br i1 %8, label %9, label %12

9:                                                ; preds = %4
  %10 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %11 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef %10, ptr noundef nonnull @"??_C@_0BG@DDAOPMNN@malloc?5failed?5for?5?$CFs?6?$AA@", ptr noundef %3) #16
  br label %90

12:                                               ; preds = %4
  tail call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 4 dereferenceable(1) %7, ptr noundef nonnull readonly align 4 dereferenceable(1) %0, i64 %6, i1 false)
  tail call void %2(ptr noundef nonnull %7, i32 noundef %1) #16, !callees !21
  %13 = icmp samesign ugt i32 %1, 1
  br i1 %13, label %14, label %26

14:                                               ; preds = %12
  %15 = zext nneg i32 %1 to i64
  br label %19

16:                                               ; preds = %19
  %17 = add nuw nsw i64 %20, 1
  %18 = icmp eq i64 %17, %15
  br i1 %18, label %26, label %19, !llvm.loop !14

19:                                               ; preds = %16, %14
  %20 = phi i64 [ 1, %14 ], [ %17, %16 ]
  %21 = getelementptr inbounds nuw i32, ptr %7, i64 %20
  %22 = load i32, ptr %21, align 4
  %23 = getelementptr i8, ptr %21, i64 -4
  %24 = load i32, ptr %23, align 4
  %25 = icmp slt i32 %22, %24
  br i1 %25, label %65, label %16

26:                                               ; preds = %16, %12
  %27 = phi i64 [ 1, %12 ], [ %15, %16 ]
  %28 = icmp samesign ult i64 %27, 4
  br i1 %28, label %61, label %29

29:                                               ; preds = %26
  %30 = and i64 %27, 16777212
  br label %31

31:                                               ; preds = %31, %29
  %32 = phi i64 [ 0, %29 ], [ %53, %31 ]
  %33 = phi <2 x i64> [ zeroinitializer, %29 ], [ %51, %31 ]
  %34 = phi <2 x i64> [ zeroinitializer, %29 ], [ %52, %31 ]
  %35 = phi <2 x i64> [ zeroinitializer, %29 ], [ %43, %31 ]
  %36 = phi <2 x i64> [ zeroinitializer, %29 ], [ %44, %31 ]
  %37 = getelementptr inbounds nuw i32, ptr %0, i64 %32
  %38 = getelementptr inbounds nuw i8, ptr %37, i64 8
  %39 = load <2 x i32>, ptr %37, align 4
  %40 = load <2 x i32>, ptr %38, align 4
  %41 = sext <2 x i32> %39 to <2 x i64>
  %42 = sext <2 x i32> %40 to <2 x i64>
  %43 = add <2 x i64> %35, %41
  %44 = add <2 x i64> %36, %42
  %45 = getelementptr inbounds nuw i32, ptr %7, i64 %32
  %46 = getelementptr inbounds nuw i8, ptr %45, i64 8
  %47 = load <2 x i32>, ptr %45, align 4
  %48 = load <2 x i32>, ptr %46, align 4
  %49 = sext <2 x i32> %47 to <2 x i64>
  %50 = sext <2 x i32> %48 to <2 x i64>
  %51 = add <2 x i64> %33, %49
  %52 = add <2 x i64> %34, %50
  %53 = add nuw i64 %32, 4
  %54 = icmp eq i64 %53, %30
  br i1 %54, label %55, label %31, !llvm.loop !22

55:                                               ; preds = %31
  %56 = add <2 x i64> %52, %51
  %57 = tail call i64 @llvm.vector.reduce.add.v2i64(<2 x i64> %56)
  %58 = add <2 x i64> %44, %43
  %59 = tail call i64 @llvm.vector.reduce.add.v2i64(<2 x i64> %58)
  %60 = icmp eq i64 %27, %30
  br i1 %60, label %68, label %61

61:                                               ; preds = %26, %55
  %62 = phi i64 [ 0, %26 ], [ %30, %55 ]
  %63 = phi i64 [ 0, %26 ], [ %57, %55 ]
  %64 = phi i64 [ 0, %26 ], [ %59, %55 ]
  br label %72

65:                                               ; preds = %19
  %66 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %67 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef %66, ptr noundef nonnull @"??_C@_0CI@BFKHFIIN@FAIL?3?5?$CFs?5did?5not?5produce?5sorted?5@", ptr noundef %3) #16
  tail call void @free(ptr noundef nonnull %7)
  br label %90

68:                                               ; preds = %72, %55
  %69 = phi i64 [ %59, %55 ], [ %79, %72 ]
  %70 = phi i64 [ %57, %55 ], [ %83, %72 ]
  %71 = icmp eq i64 %69, %70
  br i1 %71, label %89, label %86

72:                                               ; preds = %61, %72
  %73 = phi i64 [ %84, %72 ], [ %62, %61 ]
  %74 = phi i64 [ %83, %72 ], [ %63, %61 ]
  %75 = phi i64 [ %79, %72 ], [ %64, %61 ]
  %76 = getelementptr inbounds nuw i32, ptr %0, i64 %73
  %77 = load i32, ptr %76, align 4
  %78 = sext i32 %77 to i64
  %79 = add nsw i64 %75, %78
  %80 = getelementptr inbounds nuw i32, ptr %7, i64 %73
  %81 = load i32, ptr %80, align 4
  %82 = sext i32 %81 to i64
  %83 = add nsw i64 %74, %82
  %84 = add nuw nsw i64 %73, 1
  %85 = icmp eq i64 %84, %27
  br i1 %85, label %68, label %72, !llvm.loop !23

86:                                               ; preds = %68
  %87 = tail call ptr @__acrt_iob_func(i32 noundef 2) #16
  %88 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef %87, ptr noundef nonnull @"??_C@_0BP@NHICFENK@FAIL?3?5?$CFs?5element?5sum?5mismatch?6?$AA@", ptr noundef %3) #16
  tail call void @free(ptr noundef nonnull %7)
  br label %90

89:                                               ; preds = %68
  tail call void @free(ptr noundef nonnull %7)
  br label %90

90:                                               ; preds = %86, %89, %65, %9
  %91 = phi i32 [ 1, %9 ], [ 1, %65 ], [ 1, %86 ], [ 0, %89 ]
  ret i32 %91
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(ptr captures(none)) #3

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #8

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #8

; Function Attrs: noinline nounwind uwtable
define linkonce_odr dso_local ptr @__local_stdio_printf_options() local_unnamed_addr #9 comdat {
  ret ptr @__local_stdio_printf_options._OptionsStorage
}

declare dso_local i32 @__stdio_common_vfprintf(i64 noundef, ptr noundef, ptr noundef, ptr noundef, ptr noundef) local_unnamed_addr #2

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #10

; Function Attrs: nofree nosync nounwind memory(argmem: readwrite) uwtable
define internal fastcc void @quicksort_impl(ptr noundef %0, i32 noundef range(i32 -2147483646, -2147483648) %1, i32 noundef range(i32 -2147483648, 2147483647) %2) unnamed_addr #7 {
  %4 = icmp slt i32 %1, %2
  br i1 %4, label %5, label %133

5:                                                ; preds = %3, %129
  %6 = phi i32 [ %131, %129 ], [ %2, %3 ]
  %7 = phi i32 [ %130, %129 ], [ %1, %3 ]
  %8 = sub nsw i32 %6, %7
  %9 = icmp slt i32 %8, 16
  br i1 %9, label %10, label %38

10:                                               ; preds = %5
  %11 = sext i32 %7 to i64
  %12 = add nsw i32 %7, -1
  %13 = sext i32 %6 to i64
  br label %14

14:                                               ; preds = %32, %10
  %15 = phi i64 [ %11, %10 ], [ %16, %32 ]
  %16 = add nsw i64 %15, 1
  %17 = getelementptr inbounds i32, ptr %0, i64 %16
  %18 = load i32, ptr %17, align 4
  %19 = icmp slt i64 %15, %11
  %20 = trunc nsw i64 %15 to i32
  br i1 %19, label %32, label %21

21:                                               ; preds = %14, %26
  %22 = phi i64 [ %28, %26 ], [ %15, %14 ]
  %23 = getelementptr inbounds i32, ptr %0, i64 %22
  %24 = load i32, ptr %23, align 4
  %25 = icmp sgt i32 %24, %18
  br i1 %25, label %26, label %30

26:                                               ; preds = %21
  %27 = getelementptr i8, ptr %23, i64 4
  store i32 %24, ptr %27, align 4
  %28 = add nsw i64 %22, -1
  %29 = icmp sgt i64 %22, %11
  br i1 %29, label %21, label %32, !llvm.loop !24

30:                                               ; preds = %21
  %31 = trunc nsw i64 %22 to i32
  br label %32

32:                                               ; preds = %26, %30, %14
  %33 = phi i32 [ %20, %14 ], [ %31, %30 ], [ %12, %26 ]
  %34 = sext i32 %33 to i64
  %35 = getelementptr i32, ptr %0, i64 %34
  %36 = getelementptr i8, ptr %35, i64 4
  store i32 %18, ptr %36, align 4
  %37 = icmp eq i64 %16, %13
  br i1 %37, label %133, label %14, !llvm.loop !25

38:                                               ; preds = %5
  %39 = lshr i32 %8, 1
  %40 = add nsw i32 %39, %7
  %41 = sext i32 %7 to i64
  %42 = getelementptr inbounds i32, ptr %0, i64 %41
  %43 = load i32, ptr %42, align 4
  %44 = sext i32 %40 to i64
  %45 = getelementptr inbounds i32, ptr %0, i64 %44
  %46 = load i32, ptr %45, align 4
  %47 = icmp sgt i32 %43, %46
  br i1 %47, label %48, label %50

48:                                               ; preds = %38
  store i32 %46, ptr %42, align 4
  store i32 %43, ptr %45, align 4
  %49 = load i32, ptr %42, align 4
  br label %50

50:                                               ; preds = %48, %38
  %51 = phi i32 [ %43, %48 ], [ %46, %38 ]
  %52 = phi i32 [ %49, %48 ], [ %43, %38 ]
  %53 = sext i32 %6 to i64
  %54 = getelementptr inbounds i32, ptr %0, i64 %53
  %55 = load i32, ptr %54, align 4
  %56 = icmp sgt i32 %52, %55
  br i1 %56, label %57, label %59

57:                                               ; preds = %50
  store i32 %55, ptr %42, align 4
  store i32 %52, ptr %54, align 4
  %58 = load i32, ptr %45, align 4
  br label %59

59:                                               ; preds = %57, %50
  %60 = phi i32 [ %52, %57 ], [ %55, %50 ]
  %61 = phi i32 [ %58, %57 ], [ %51, %50 ]
  %62 = icmp sgt i32 %61, %60
  br i1 %62, label %63, label %65

63:                                               ; preds = %59
  store i32 %60, ptr %45, align 4
  store i32 %61, ptr %54, align 4
  %64 = load i32, ptr %45, align 4
  br label %65

65:                                               ; preds = %59, %63
  %66 = phi i32 [ %60, %59 ], [ %61, %63 ]
  %67 = phi i32 [ %61, %59 ], [ %64, %63 ]
  store i32 %66, ptr %45, align 4
  store i32 %67, ptr %54, align 4
  %68 = add nsw i32 %7, -1
  %69 = sub nsw i64 %53, %41
  %70 = and i64 %69, 1
  %71 = icmp eq i64 %70, 0
  br i1 %71, label %83, label %72

72:                                               ; preds = %65
  %73 = getelementptr inbounds i32, ptr %0, i64 %41
  %74 = load i32, ptr %73, align 4
  %75 = icmp sgt i32 %74, %67
  br i1 %75, label %80, label %76

76:                                               ; preds = %72
  %77 = sext i32 %7 to i64
  %78 = getelementptr inbounds i32, ptr %0, i64 %77
  %79 = load i32, ptr %78, align 4
  store i32 %74, ptr %78, align 4
  store i32 %79, ptr %73, align 4
  br label %80

80:                                               ; preds = %76, %72
  %81 = phi i32 [ %7, %76 ], [ %68, %72 ]
  %82 = add nsw i64 %41, 1
  br label %83

83:                                               ; preds = %80, %65
  %84 = phi i32 [ poison, %65 ], [ %81, %80 ]
  %85 = phi i64 [ %41, %65 ], [ %82, %80 ]
  %86 = phi i32 [ %68, %65 ], [ %81, %80 ]
  %87 = add nsw i64 %53, -1
  %88 = icmp eq i64 %87, %41
  br i1 %88, label %115, label %89

89:                                               ; preds = %83, %111
  %90 = phi i64 [ %113, %111 ], [ %85, %83 ]
  %91 = phi i32 [ %112, %111 ], [ %86, %83 ]
  %92 = getelementptr inbounds i32, ptr %0, i64 %90
  %93 = load i32, ptr %92, align 4
  %94 = icmp sgt i32 %93, %67
  br i1 %94, label %100, label %95

95:                                               ; preds = %89
  %96 = add nsw i32 %91, 1
  %97 = sext i32 %96 to i64
  %98 = getelementptr inbounds i32, ptr %0, i64 %97
  %99 = load i32, ptr %98, align 4
  store i32 %93, ptr %98, align 4
  store i32 %99, ptr %92, align 4
  br label %100

100:                                              ; preds = %95, %89
  %101 = phi i32 [ %96, %95 ], [ %91, %89 ]
  %102 = getelementptr i32, ptr %0, i64 %90
  %103 = getelementptr i8, ptr %102, i64 4
  %104 = load i32, ptr %103, align 4
  %105 = icmp sgt i32 %104, %67
  br i1 %105, label %111, label %106

106:                                              ; preds = %100
  %107 = add nsw i32 %101, 1
  %108 = sext i32 %107 to i64
  %109 = getelementptr inbounds i32, ptr %0, i64 %108
  %110 = load i32, ptr %109, align 4
  store i32 %104, ptr %109, align 4
  store i32 %110, ptr %103, align 4
  br label %111

111:                                              ; preds = %106, %100
  %112 = phi i32 [ %107, %106 ], [ %101, %100 ]
  %113 = add nsw i64 %90, 2
  %114 = icmp eq i64 %113, %53
  br i1 %114, label %115, label %89, !llvm.loop !26

115:                                              ; preds = %111, %83
  %116 = phi i32 [ %84, %83 ], [ %112, %111 ]
  %117 = load i32, ptr %54, align 4
  %118 = add nsw i32 %116, 1
  %119 = sext i32 %118 to i64
  %120 = getelementptr inbounds i32, ptr %0, i64 %119
  %121 = load i32, ptr %120, align 4
  store i32 %117, ptr %120, align 4
  store i32 %121, ptr %54, align 4
  %122 = sub nsw i32 %118, %7
  %123 = sub nsw i32 %6, %118
  %124 = icmp slt i32 %122, %123
  br i1 %124, label %125, label %127

125:                                              ; preds = %115
  tail call fastcc void @quicksort_impl(ptr noundef nonnull %0, i32 noundef %7, i32 noundef %116)
  %126 = add nsw i32 %116, 2
  br label %129

127:                                              ; preds = %115
  %128 = add nsw i32 %116, 2
  tail call fastcc void @quicksort_impl(ptr noundef nonnull %0, i32 noundef %128, i32 noundef %6)
  br label %129

129:                                              ; preds = %127, %125
  %130 = phi i32 [ %126, %125 ], [ %7, %127 ]
  %131 = phi i32 [ %6, %125 ], [ %116, %127 ]
  %132 = icmp slt i32 %130, %131
  br i1 %132, label %5, label %133, !llvm.loop !27

133:                                              ; preds = %129, %32, %3
  ret void
}

; Function Attrs: nofree noreturn
declare dso_local void @exit(i32 noundef) local_unnamed_addr #11

; Function Attrs: nofree nosync nounwind memory(argmem: readwrite) uwtable
define internal fastcc void @mergesort_impl(ptr noundef %0, ptr noundef nonnull %1, i32 noundef range(i32 0, -2147483648) %2, i32 noundef range(i32 1, -2147483648) %3) unnamed_addr #7 {
  %5 = ptrtoint ptr %0 to i64
  %6 = ptrtoint ptr %1 to i64
  %7 = sub nsw i32 %3, %2
  %8 = icmp slt i32 %7, 16
  br i1 %8, label %9, label %39

9:                                                ; preds = %4
  %10 = icmp samesign ult i32 %2, %3
  br i1 %10, label %11, label %320

11:                                               ; preds = %9
  %12 = zext nneg i32 %2 to i64
  %13 = add nsw i32 %2, -1
  %14 = zext nneg i32 %3 to i64
  br label %15

15:                                               ; preds = %33, %11
  %16 = phi i64 [ %12, %11 ], [ %17, %33 ]
  %17 = add nuw nsw i64 %16, 1
  %18 = getelementptr inbounds nuw i32, ptr %0, i64 %17
  %19 = load i32, ptr %18, align 4
  %20 = icmp samesign ult i64 %16, %12
  %21 = trunc nsw i64 %16 to i32
  br i1 %20, label %33, label %22

22:                                               ; preds = %15, %27
  %23 = phi i64 [ %29, %27 ], [ %16, %15 ]
  %24 = getelementptr inbounds i32, ptr %0, i64 %23
  %25 = load i32, ptr %24, align 4
  %26 = icmp sgt i32 %25, %19
  br i1 %26, label %27, label %31

27:                                               ; preds = %22
  %28 = getelementptr i8, ptr %24, i64 4
  store i32 %25, ptr %28, align 4
  %29 = add nsw i64 %23, -1
  %30 = icmp sgt i64 %23, %12
  br i1 %30, label %22, label %33, !llvm.loop !24

31:                                               ; preds = %22
  %32 = trunc nsw i64 %23 to i32
  br label %33

33:                                               ; preds = %27, %31, %15
  %34 = phi i32 [ %21, %15 ], [ %32, %31 ], [ %13, %27 ]
  %35 = sext i32 %34 to i64
  %36 = getelementptr i32, ptr %0, i64 %35
  %37 = getelementptr i8, ptr %36, i64 4
  store i32 %19, ptr %37, align 4
  %38 = icmp eq i64 %17, %14
  br i1 %38, label %320, label %15, !llvm.loop !25

39:                                               ; preds = %4
  %40 = lshr i32 %7, 1
  %41 = add nuw nsw i32 %40, %2
  tail call fastcc void @mergesort_impl(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %41)
  %42 = add nuw nsw i32 %41, 1
  tail call fastcc void @mergesort_impl(ptr noundef %0, ptr noundef %1, i32 noundef %42, i32 noundef %3)
  %43 = zext nneg i32 %41 to i64
  %44 = getelementptr inbounds nuw i32, ptr %0, i64 %43
  %45 = load i32, ptr %44, align 4
  %46 = zext nneg i32 %42 to i64
  %47 = getelementptr inbounds nuw i32, ptr %0, i64 %46
  %48 = load i32, ptr %47, align 4
  %49 = icmp sgt i32 %45, %48
  br i1 %49, label %50, label %320

50:                                               ; preds = %39
  %51 = icmp samesign ult i32 %41, %3
  br i1 %51, label %52, label %57

52:                                               ; preds = %50
  %53 = zext nneg i32 %2 to i64
  br label %119

54:                                               ; preds = %119
  %55 = trunc nuw i64 %137 to i32
  %56 = icmp samesign ugt i32 %132, %41
  br i1 %56, label %141, label %57

57:                                               ; preds = %50, %54
  %58 = phi i32 [ %55, %54 ], [ %2, %50 ]
  %59 = phi i32 [ %135, %54 ], [ %42, %50 ]
  %60 = phi i32 [ %132, %54 ], [ %2, %50 ]
  %61 = zext i32 %58 to i64
  %62 = zext i32 %60 to i64
  %63 = add nuw i32 %2, %40
  %64 = sub i32 %63, %60
  %65 = zext i32 %64 to i64
  %66 = add nuw nsw i64 %65, 1
  %67 = icmp ult i32 %64, 11
  br i1 %67, label %93, label %68

68:                                               ; preds = %57
  %69 = shl nuw nsw i64 %61, 2
  %70 = add i64 %69, %6
  %71 = shl nuw nsw i64 %62, 2
  %72 = add i64 %71, %5
  %73 = sub i64 %70, %72
  %74 = icmp ult i64 %73, 32
  br i1 %74, label %93, label %75

75:                                               ; preds = %68
  %76 = and i64 %66, 8589934584
  %77 = add nuw nsw i64 %76, %62
  %78 = add nuw nsw i64 %76, %61
  %79 = getelementptr i32, ptr %0, i64 %62
  %80 = getelementptr i32, ptr %1, i64 %61
  br label %81

81:                                               ; preds = %81, %75
  %82 = phi i64 [ 0, %75 ], [ %89, %81 ]
  %83 = getelementptr i32, ptr %79, i64 %82
  %84 = getelementptr inbounds nuw i8, ptr %83, i64 16
  %85 = load <4 x i32>, ptr %83, align 4
  %86 = load <4 x i32>, ptr %84, align 4
  %87 = getelementptr i32, ptr %80, i64 %82
  %88 = getelementptr inbounds nuw i8, ptr %87, i64 16
  store <4 x i32> %85, ptr %87, align 4
  store <4 x i32> %86, ptr %88, align 4
  %89 = add nuw i64 %82, 8
  %90 = icmp eq i64 %89, %76
  br i1 %90, label %91, label %81, !llvm.loop !28

91:                                               ; preds = %81
  %92 = icmp eq i64 %66, %76
  br i1 %92, label %141, label %93

93:                                               ; preds = %68, %57, %91
  %94 = phi i64 [ %62, %68 ], [ %62, %57 ], [ %77, %91 ]
  %95 = phi i64 [ %61, %68 ], [ %61, %57 ], [ %78, %91 ]
  %96 = add nuw i32 %2, %40
  %97 = add i32 %96, 1
  %98 = trunc i64 %94 to i32
  %99 = sub i32 %97, %98
  %100 = sub i32 %96, %98
  %101 = and i32 %99, 3
  %102 = icmp eq i32 %101, 0
  br i1 %102, label %114, label %103

103:                                              ; preds = %93, %103
  %104 = phi i64 [ %107, %103 ], [ %94, %93 ]
  %105 = phi i64 [ %110, %103 ], [ %95, %93 ]
  %106 = phi i32 [ %112, %103 ], [ 0, %93 ]
  %107 = add nuw nsw i64 %104, 1
  %108 = getelementptr inbounds nuw i32, ptr %0, i64 %104
  %109 = load i32, ptr %108, align 4
  %110 = add nuw nsw i64 %105, 1
  %111 = getelementptr inbounds nuw i32, ptr %1, i64 %105
  store i32 %109, ptr %111, align 4
  %112 = add i32 %106, 1
  %113 = icmp eq i32 %112, %101
  br i1 %113, label %114, label %103, !llvm.loop !29

114:                                              ; preds = %103, %93
  %115 = phi i64 [ poison, %93 ], [ %110, %103 ]
  %116 = phi i64 [ %94, %93 ], [ %107, %103 ]
  %117 = phi i64 [ %95, %93 ], [ %110, %103 ]
  %118 = icmp ult i32 %100, 3
  br i1 %118, label %141, label %202

119:                                              ; preds = %119, %52
  %120 = phi i64 [ %53, %52 ], [ %137, %119 ]
  %121 = phi i32 [ %42, %52 ], [ %135, %119 ]
  %122 = phi i32 [ %2, %52 ], [ %132, %119 ]
  %123 = zext nneg i32 %122 to i64
  %124 = getelementptr inbounds nuw i32, ptr %0, i64 %123
  %125 = load i32, ptr %124, align 4
  %126 = zext nneg i32 %121 to i64
  %127 = getelementptr inbounds nuw i32, ptr %0, i64 %126
  %128 = load i32, ptr %127, align 4
  %129 = icmp sle i32 %125, %128
  %130 = tail call i32 @llvm.smin.i32(i32 %125, i32 %128)
  %131 = zext i1 %129 to i32
  %132 = add nuw nsw i32 %122, %131
  %133 = xor i1 %129, true
  %134 = zext i1 %133 to i32
  %135 = add nuw nsw i32 %121, %134
  %136 = getelementptr inbounds nuw i32, ptr %1, i64 %120
  store i32 %130, ptr %136, align 4
  %137 = add nuw nsw i64 %120, 1
  %138 = icmp samesign ule i32 %132, %41
  %139 = icmp samesign ule i32 %135, %3
  %140 = select i1 %138, i1 %139, i1 false
  br i1 %140, label %119, label %54, !llvm.loop !31

141:                                              ; preds = %114, %202, %91, %54
  %142 = phi i32 [ %135, %54 ], [ %59, %91 ], [ %59, %202 ], [ %59, %114 ]
  %143 = phi i64 [ %137, %54 ], [ %78, %91 ], [ %115, %114 ], [ %222, %202 ]
  %144 = icmp samesign ugt i32 %142, %3
  br i1 %144, label %227, label %145

145:                                              ; preds = %141
  %146 = and i64 %143, 4294967295
  %147 = zext nneg i32 %142 to i64
  %148 = add nuw i32 %3, 1
  %149 = sub nuw i32 %3, %142
  %150 = add nuw i32 %149, 1
  %151 = zext i32 %150 to i64
  %152 = icmp samesign ult i32 %149, 11
  br i1 %152, label %178, label %153

153:                                              ; preds = %145
  %154 = shl nuw nsw i64 %146, 2
  %155 = add i64 %154, %6
  %156 = shl nuw nsw i64 %147, 2
  %157 = add i64 %156, %5
  %158 = sub i64 %155, %157
  %159 = icmp ult i64 %158, 32
  br i1 %159, label %178, label %160

160:                                              ; preds = %153
  %161 = and i64 %151, 4294967288
  %162 = add nuw nsw i64 %161, %147
  %163 = add nuw nsw i64 %146, %161
  %164 = getelementptr i32, ptr %0, i64 %147
  %165 = getelementptr i32, ptr %1, i64 %146
  br label %166

166:                                              ; preds = %166, %160
  %167 = phi i64 [ 0, %160 ], [ %174, %166 ]
  %168 = getelementptr i32, ptr %164, i64 %167
  %169 = getelementptr inbounds nuw i8, ptr %168, i64 16
  %170 = load <4 x i32>, ptr %168, align 4
  %171 = load <4 x i32>, ptr %169, align 4
  %172 = getelementptr i32, ptr %165, i64 %167
  %173 = getelementptr inbounds nuw i8, ptr %172, i64 16
  store <4 x i32> %170, ptr %172, align 4
  store <4 x i32> %171, ptr %173, align 4
  %174 = add nuw i64 %167, 8
  %175 = icmp eq i64 %174, %161
  br i1 %175, label %176, label %166, !llvm.loop !32

176:                                              ; preds = %166
  %177 = icmp eq i64 %161, %151
  br i1 %177, label %227, label %178

178:                                              ; preds = %153, %145, %176
  %179 = phi i64 [ %147, %153 ], [ %147, %145 ], [ %162, %176 ]
  %180 = phi i64 [ %146, %153 ], [ %146, %145 ], [ %163, %176 ]
  %181 = add nuw i32 %3, 1
  %182 = trunc i64 %179 to i32
  %183 = sub i32 %181, %182
  %184 = sub i32 %3, %182
  %185 = and i32 %183, 3
  %186 = icmp eq i32 %185, 0
  br i1 %186, label %198, label %187

187:                                              ; preds = %178, %187
  %188 = phi i64 [ %191, %187 ], [ %179, %178 ]
  %189 = phi i64 [ %194, %187 ], [ %180, %178 ]
  %190 = phi i32 [ %196, %187 ], [ 0, %178 ]
  %191 = add nuw nsw i64 %188, 1
  %192 = getelementptr inbounds nuw i32, ptr %0, i64 %188
  %193 = load i32, ptr %192, align 4
  %194 = add nuw nsw i64 %189, 1
  %195 = getelementptr inbounds nuw i32, ptr %1, i64 %189
  store i32 %193, ptr %195, align 4
  %196 = add i32 %190, 1
  %197 = icmp eq i32 %196, %185
  br i1 %197, label %198, label %187, !llvm.loop !33

198:                                              ; preds = %187, %178
  %199 = phi i64 [ %179, %178 ], [ %191, %187 ]
  %200 = phi i64 [ %180, %178 ], [ %194, %187 ]
  %201 = icmp ult i32 %184, 3
  br i1 %201, label %227, label %276

202:                                              ; preds = %114, %202
  %203 = phi i64 [ %218, %202 ], [ %116, %114 ]
  %204 = phi i64 [ %222, %202 ], [ %117, %114 ]
  %205 = getelementptr inbounds nuw i32, ptr %0, i64 %203
  %206 = load i32, ptr %205, align 4
  %207 = getelementptr inbounds nuw i32, ptr %1, i64 %204
  store i32 %206, ptr %207, align 4
  %208 = getelementptr inbounds nuw i32, ptr %0, i64 %203
  %209 = getelementptr inbounds nuw i8, ptr %208, i64 4
  %210 = load i32, ptr %209, align 4
  %211 = getelementptr inbounds nuw i32, ptr %1, i64 %204
  %212 = getelementptr inbounds nuw i8, ptr %211, i64 4
  store i32 %210, ptr %212, align 4
  %213 = getelementptr inbounds nuw i32, ptr %0, i64 %203
  %214 = getelementptr inbounds nuw i8, ptr %213, i64 8
  %215 = load i32, ptr %214, align 4
  %216 = getelementptr inbounds nuw i32, ptr %1, i64 %204
  %217 = getelementptr inbounds nuw i8, ptr %216, i64 8
  store i32 %215, ptr %217, align 4
  %218 = add nuw nsw i64 %203, 4
  %219 = getelementptr inbounds nuw i32, ptr %0, i64 %203
  %220 = getelementptr inbounds nuw i8, ptr %219, i64 12
  %221 = load i32, ptr %220, align 4
  %222 = add nuw nsw i64 %204, 4
  %223 = getelementptr inbounds nuw i32, ptr %1, i64 %204
  %224 = getelementptr inbounds nuw i8, ptr %223, i64 12
  store i32 %221, ptr %224, align 4
  %225 = trunc i64 %218 to i32
  %226 = icmp eq i32 %42, %225
  br i1 %226, label %141, label %202, !llvm.loop !34

227:                                              ; preds = %198, %276, %176, %141
  %228 = icmp samesign ugt i32 %2, %3
  br i1 %228, label %320, label %229

229:                                              ; preds = %227
  %230 = zext nneg i32 %2 to i64
  %231 = add nuw i32 %3, 1
  %232 = zext i32 %231 to i64
  %233 = add nuw i32 %3, 1
  %234 = sub nuw i32 %233, %2
  %235 = zext i32 %234 to i64
  %236 = icmp ult i32 %234, 8
  %237 = sub i64 %5, %6
  %238 = icmp ult i64 %237, 32
  %239 = or i1 %236, %238
  br i1 %239, label %256, label %240

240:                                              ; preds = %229
  %241 = and i64 %235, 4294967288
  %242 = add nuw nsw i64 %241, %230
  br label %243

243:                                              ; preds = %243, %240
  %244 = phi i64 [ 0, %240 ], [ %252, %243 ]
  %245 = add i64 %244, %230
  %246 = getelementptr inbounds nuw i32, ptr %1, i64 %245
  %247 = getelementptr inbounds nuw i8, ptr %246, i64 16
  %248 = load <4 x i32>, ptr %246, align 4
  %249 = load <4 x i32>, ptr %247, align 4
  %250 = getelementptr inbounds nuw i32, ptr %0, i64 %245
  %251 = getelementptr inbounds nuw i8, ptr %250, i64 16
  store <4 x i32> %248, ptr %250, align 4
  store <4 x i32> %249, ptr %251, align 4
  %252 = add nuw i64 %244, 8
  %253 = icmp eq i64 %252, %241
  br i1 %253, label %254, label %243, !llvm.loop !35

254:                                              ; preds = %243
  %255 = icmp eq i64 %241, %235
  br i1 %255, label %320, label %256

256:                                              ; preds = %229, %254
  %257 = phi i64 [ %230, %229 ], [ %242, %254 ]
  %258 = zext nneg i32 %3 to i64
  %259 = add nuw nsw i64 %258, 1
  %260 = sub nsw i64 %259, %257
  %261 = sub nsw i64 %258, %257
  %262 = and i64 %260, 3
  %263 = icmp eq i64 %262, 0
  br i1 %263, label %273, label %264

264:                                              ; preds = %256, %264
  %265 = phi i64 [ %270, %264 ], [ %257, %256 ]
  %266 = phi i64 [ %271, %264 ], [ 0, %256 ]
  %267 = getelementptr inbounds nuw i32, ptr %1, i64 %265
  %268 = load i32, ptr %267, align 4
  %269 = getelementptr inbounds nuw i32, ptr %0, i64 %265
  store i32 %268, ptr %269, align 4
  %270 = add nuw nsw i64 %265, 1
  %271 = add i64 %266, 1
  %272 = icmp eq i64 %271, %262
  br i1 %272, label %273, label %264, !llvm.loop !36

273:                                              ; preds = %264, %256
  %274 = phi i64 [ %257, %256 ], [ %270, %264 ]
  %275 = icmp ult i64 %261, 3
  br i1 %275, label %320, label %301

276:                                              ; preds = %198, %276
  %277 = phi i64 [ %292, %276 ], [ %199, %198 ]
  %278 = phi i64 [ %296, %276 ], [ %200, %198 ]
  %279 = getelementptr inbounds nuw i32, ptr %0, i64 %277
  %280 = load i32, ptr %279, align 4
  %281 = getelementptr inbounds nuw i32, ptr %1, i64 %278
  store i32 %280, ptr %281, align 4
  %282 = getelementptr inbounds nuw i32, ptr %0, i64 %277
  %283 = getelementptr inbounds nuw i8, ptr %282, i64 4
  %284 = load i32, ptr %283, align 4
  %285 = getelementptr inbounds nuw i32, ptr %1, i64 %278
  %286 = getelementptr inbounds nuw i8, ptr %285, i64 4
  store i32 %284, ptr %286, align 4
  %287 = getelementptr inbounds nuw i32, ptr %0, i64 %277
  %288 = getelementptr inbounds nuw i8, ptr %287, i64 8
  %289 = load i32, ptr %288, align 4
  %290 = getelementptr inbounds nuw i32, ptr %1, i64 %278
  %291 = getelementptr inbounds nuw i8, ptr %290, i64 8
  store i32 %289, ptr %291, align 4
  %292 = add nuw nsw i64 %277, 4
  %293 = getelementptr inbounds nuw i32, ptr %0, i64 %277
  %294 = getelementptr inbounds nuw i8, ptr %293, i64 12
  %295 = load i32, ptr %294, align 4
  %296 = add nuw nsw i64 %278, 4
  %297 = getelementptr inbounds nuw i32, ptr %1, i64 %278
  %298 = getelementptr inbounds nuw i8, ptr %297, i64 12
  store i32 %295, ptr %298, align 4
  %299 = trunc i64 %292 to i32
  %300 = icmp eq i32 %148, %299
  br i1 %300, label %227, label %276, !llvm.loop !37

301:                                              ; preds = %273, %301
  %302 = phi i64 [ %318, %301 ], [ %274, %273 ]
  %303 = getelementptr inbounds nuw i32, ptr %1, i64 %302
  %304 = load i32, ptr %303, align 4
  %305 = getelementptr inbounds nuw i32, ptr %0, i64 %302
  store i32 %304, ptr %305, align 4
  %306 = add nuw nsw i64 %302, 1
  %307 = getelementptr inbounds nuw i32, ptr %1, i64 %306
  %308 = load i32, ptr %307, align 4
  %309 = getelementptr inbounds nuw i32, ptr %0, i64 %306
  store i32 %308, ptr %309, align 4
  %310 = add nuw nsw i64 %302, 2
  %311 = getelementptr inbounds nuw i32, ptr %1, i64 %310
  %312 = load i32, ptr %311, align 4
  %313 = getelementptr inbounds nuw i32, ptr %0, i64 %310
  store i32 %312, ptr %313, align 4
  %314 = add nuw nsw i64 %302, 3
  %315 = getelementptr inbounds nuw i32, ptr %1, i64 %314
  %316 = load i32, ptr %315, align 4
  %317 = getelementptr inbounds nuw i32, ptr %0, i64 %314
  store i32 %316, ptr %317, align 4
  %318 = add nuw nsw i64 %302, 4
  %319 = icmp eq i64 %318, %232
  br i1 %319, label %320, label %301, !llvm.loop !38

320:                                              ; preds = %273, %301, %33, %254, %227, %9, %39
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #12

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #12

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.fshl.i32(i32, i32, i32) #13

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smin.i32(i32, i32) #13

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #13

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare <2 x i32> @llvm.smin.v2i32(<2 x i32>, <2 x i32>) #13

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare <2 x i32> @llvm.smax.v2i32(<2 x i32>, <2 x i32>) #13

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.smax.v2i32(<2 x i32>) #14

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.smin.v2i32(<2 x i32>) #14

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.vector.reduce.add.v2i64(<2 x i64>) #14

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.or.v4i32(<4 x i32>) #14

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.and.v4i32(<4 x i32>) #14

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.xor.v4i32(<4 x i32>) #14

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef) #15

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { inlinehint nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { mustprogress nocallback nofree nounwind willreturn memory(read) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nofree nosync nounwind memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { mustprogress nocallback nofree nosync nounwind willreturn }
attributes #9 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #10 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #11 = { nofree noreturn "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #12 = { nofree nounwind }
attributes #13 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #14 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #15 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #16 = { nounwind }
attributes #17 = { allocsize(0) }
attributes #18 = { cold noreturn nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 22.1.2 (https://github.com/llvm/llvm-project 1ab49a973e210e97d61e5db6557180dcb92c3e98)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "sort.c", directory: "C:\\Users\\Treverm\\Documents\\Claude Code\\llvm-obfuscator\\eval\\benchmarks")
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 2}
!4 = !{i32 8, !"PIC Level", i32 2}
!5 = !{i32 7, !"uwtable", i32 2}
!6 = !{i32 1, !"MaxTLSAlign", i32 65536}
!7 = !{!"clang version 22.1.2 (https://github.com/llvm/llvm-project 1ab49a973e210e97d61e5db6557180dcb92c3e98)"}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.mustprogress"}
!10 = distinct !{!10, !9, !11, !12}
!11 = !{!"llvm.loop.isvectorized", i32 1}
!12 = !{!"llvm.loop.unroll.runtime.disable"}
!13 = distinct !{!13, !9, !12, !11}
!14 = distinct !{!14, !9}
!15 = distinct !{!15, !9}
!16 = distinct !{!16, !9}
!17 = distinct !{!17, !9, !11, !12}
!18 = distinct !{!18, !9, !12, !11}
!19 = distinct !{!19, !9, !11, !12}
!20 = distinct !{!20, !9, !12, !11}
!21 = !{ptr @mergesort, ptr @quicksort}
!22 = distinct !{!22, !9, !11, !12}
!23 = distinct !{!23, !9, !12, !11}
!24 = distinct !{!24, !9}
!25 = distinct !{!25, !9}
!26 = distinct !{!26, !9}
!27 = distinct !{!27, !9}
!28 = distinct !{!28, !9, !11, !12}
!29 = distinct !{!29, !30}
!30 = !{!"llvm.loop.unroll.disable"}
!31 = distinct !{!31, !9}
!32 = distinct !{!32, !9, !11, !12}
!33 = distinct !{!33, !30}
!34 = distinct !{!34, !9, !11}
!35 = distinct !{!35, !9, !11, !12}
!36 = distinct !{!36, !30}
!37 = distinct !{!37, !9, !11}
!38 = distinct !{!38, !9, !11}
