; ModuleID = 'rc4.c'
source_filename = "rc4.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.44.35224"

%struct.RC4State = type { [256 x i8], i8, i8 }

$fprintf = comdat any

$printf = comdat any

$__local_stdio_printf_options = comdat any

$"??_C@_0BN@DBFPDJKO@Usage?3?5?$CFs?5?$DMkey?$DO?5?$DMplaintext?$DO?6?$AA@" = comdat any

$"??_C@_0BJ@BBKLPIEG@Key?5must?5be?51?9256?5bytes?6?$AA@" = comdat any

$"??_C@_0BN@FACGIDNE@Plaintext?5must?5not?5be?5empty?6?$AA@" = comdat any

$"??_C@_0BG@EMBMJCIM@Key?5length?3?5?$CFd?5bytes?6?$AA@" = comdat any

$"??_C@_0BM@CLAPAJPL@Plaintext?5length?3?5?$CFd?5bytes?6?$AA@" = comdat any

$"??_C@_0P@LEGHBOFO@malloc?5failed?6?$AA@" = comdat any

$"??_C@_0N@FADNOEAA@Ciphertext?3?5?$AA@" = comdat any

$"??_C@_0BD@GHNLPNIN@CT?5checksum?3?5?$CF08x?6?$AA@" = comdat any

$"??_C@_0CP@LEKLIDOD@FAIL?3?5Decrypted?5text?5does?5not?5ma@" = comdat any

$"??_C@_0BA@IDGDPNHB@?5?5Expected?3?5?$CFs?6?$AA@" = comdat any

$"??_C@_0BA@GKHOJLAE@?5?5Got?3?5?5?5?5?5?5?$CFs?6?$AA@" = comdat any

$"??_C@_0DB@KEHBAICJ@FAIL?3?5XOR?5string?5encode?1decode?5r@" = comdat any

$"??_C@_0CK@EPPHFHAG@FAIL?3?5Multi?9round?5encrypt?1decryp@" = comdat any

$"??_C@_04NOJCDH@?$CF02x?$AA@" = comdat any

$"??_C@_0CC@JCHMFFP@?5?5Keystream?5analysis?5?$CI?$CFd?5bytes?$CJ?3@" = comdat any

$"??_C@_0DB@MCFGGKFC@?5?5Chi?9squared?3?5?$CF?42f?5?$CIexpected?5?$HO2@" = comdat any

$"??_C@_0CF@BDAJAHGO@?5?5Total?5ones?3?5?$CFu?5?1?5?$CFu?5bits?5?$CI?$CF?41f@" = comdat any

@"??_C@_0BN@DBFPDJKO@Usage?3?5?$CFs?5?$DMkey?$DO?5?$DMplaintext?$DO?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [29 x i8] c"Usage: %s <key> <plaintext>\0A\00", comdat, align 1
@"??_C@_0BJ@BBKLPIEG@Key?5must?5be?51?9256?5bytes?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [25 x i8] c"Key must be 1-256 bytes\0A\00", comdat, align 1
@"??_C@_0BN@FACGIDNE@Plaintext?5must?5not?5be?5empty?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [29 x i8] c"Plaintext must not be empty\0A\00", comdat, align 1
@"??_C@_0BG@EMBMJCIM@Key?5length?3?5?$CFd?5bytes?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [22 x i8] c"Key length: %d bytes\0A\00", comdat, align 1
@"??_C@_0BM@CLAPAJPL@Plaintext?5length?3?5?$CFd?5bytes?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [28 x i8] c"Plaintext length: %d bytes\0A\00", comdat, align 1
@"??_C@_0P@LEGHBOFO@malloc?5failed?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"malloc failed\0A\00", comdat, align 1
@"??_C@_0N@FADNOEAA@Ciphertext?3?5?$AA@" = linkonce_odr dso_local unnamed_addr constant [13 x i8] c"Ciphertext: \00", comdat, align 1
@"??_C@_0BD@GHNLPNIN@CT?5checksum?3?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [19 x i8] c"CT checksum: %08x\0A\00", comdat, align 1
@"??_C@_0CP@LEKLIDOD@FAIL?3?5Decrypted?5text?5does?5not?5ma@" = linkonce_odr dso_local unnamed_addr constant [47 x i8] c"FAIL: Decrypted text does not match plaintext\0A\00", comdat, align 1
@"??_C@_0BA@IDGDPNHB@?5?5Expected?3?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [16 x i8] c"  Expected: %s\0A\00", comdat, align 1
@"??_C@_0BA@GKHOJLAE@?5?5Got?3?5?5?5?5?5?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [16 x i8] c"  Got:      %s\0A\00", comdat, align 1
@"??_C@_0DB@KEHBAICJ@FAIL?3?5XOR?5string?5encode?1decode?5r@" = linkonce_odr dso_local unnamed_addr constant [49 x i8] c"FAIL: XOR string encode/decode roundtrip failed\0A\00", comdat, align 1
@"??_C@_0CK@EPPHFHAG@FAIL?3?5Multi?9round?5encrypt?1decryp@" = linkonce_odr dso_local unnamed_addr constant [42 x i8] c"FAIL: Multi-round encrypt/decrypt failed\0A\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@"??_C@_04NOJCDH@?$CF02x?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"%02x\00", comdat, align 1
@"??_C@_0CC@JCHMFFP@?5?5Keystream?5analysis?5?$CI?$CFd?5bytes?$CJ?3@" = linkonce_odr dso_local unnamed_addr constant [34 x i8] c"  Keystream analysis (%d bytes):\0A\00", comdat, align 1
@"??_C@_0DB@MCFGGKFC@?5?5Chi?9squared?3?5?$CF?42f?5?$CIexpected?5?$HO2@" = linkonce_odr dso_local unnamed_addr constant [49 x i8] c"  Chi-squared: %.2f (expected ~255 for uniform)\0A\00", comdat, align 1
@"??_C@_0CF@BDAJAHGO@?5?5Total?5ones?3?5?$CFu?5?1?5?$CFu?5bits?5?$CI?$CF?41f@" = linkonce_odr dso_local unnamed_addr constant [37 x i8] c"  Total ones: %u / %u bits (%.1f%%)\0A\00", comdat, align 1
@str = private unnamed_addr constant [22 x i8] c"=== RC4 Benchmark ===\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"Multi-round test:\00", align 1
@str.2 = private unnamed_addr constant [34 x i8] c"  20 rounds (10 enc + 10 dec): OK\00", align 1
@str.3 = private unnamed_addr constant [13 x i8] c"RESULT: PASS\00", align 1
@str.4 = private unnamed_addr constant [13 x i8] c"RESULT: FAIL\00", align 1

; Function Attrs: nounwind uwtable
define dso_local range(i32 0, 2) i32 @main(i32 noundef %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #0 {
  %3 = alloca [256 x i32], align 16
  %4 = alloca %struct.RC4State, align 1
  %5 = alloca %struct.RC4State, align 16
  %6 = alloca %struct.RC4State, align 1
  %7 = alloca %struct.RC4State, align 16
  %8 = alloca [256 x i8], align 16
  %9 = alloca [256 x i8], align 16
  %10 = alloca %struct.RC4State, align 16
  %11 = alloca %struct.RC4State, align 16
  %12 = icmp eq i32 %0, 3
  br i1 %12, label %17, label %13

13:                                               ; preds = %2
  %14 = load ptr, ptr %1, align 8
  %15 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %16 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef %15, ptr noundef nonnull @"??_C@_0BN@DBFPDJKO@Usage?3?5?$CFs?5?$DMkey?$DO?5?$DMplaintext?$DO?6?$AA@", ptr noundef %14) #14
  br label %667

17:                                               ; preds = %2
  %18 = getelementptr inbounds nuw i8, ptr %1, i64 8
  %19 = load ptr, ptr %18, align 8
  %20 = getelementptr inbounds nuw i8, ptr %1, i64 16
  %21 = load ptr, ptr %20, align 8
  %22 = tail call i64 @strlen(ptr noundef nonnull dereferenceable(1) %19) #14
  %23 = trunc i64 %22 to i32
  %24 = tail call i64 @strlen(ptr noundef nonnull dereferenceable(1) %21) #14
  %25 = trunc i64 %24 to i32
  %26 = icmp eq i32 %23, 0
  %27 = icmp sgt i32 %23, 256
  %28 = or i1 %26, %27
  br i1 %28, label %29, label %32

29:                                               ; preds = %17
  %30 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %31 = tail call i64 @fwrite(ptr nonnull @"??_C@_0BJ@BBKLPIEG@Key?5must?5be?51?9256?5bytes?6?$AA@", i64 24, i64 1, ptr %30)
  br label %667

32:                                               ; preds = %17
  %33 = icmp eq i32 %25, 0
  br i1 %33, label %34, label %37

34:                                               ; preds = %32
  %35 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %36 = tail call i64 @fwrite(ptr nonnull @"??_C@_0BN@FACGIDNE@Plaintext?5must?5not?5be?5empty?6?$AA@", i64 28, i64 1, ptr %35)
  br label %667

37:                                               ; preds = %32
  %38 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %39 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BG@EMBMJCIM@Key?5length?3?5?$CFd?5bytes?6?$AA@", i32 noundef %23)
  %40 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BM@CLAPAJPL@Plaintext?5length?3?5?$CFd?5bytes?6?$AA@", i32 noundef %25)
  call void @llvm.lifetime.start.p0(ptr nonnull %5) #14
  %41 = getelementptr inbounds nuw i8, ptr %5, i64 16
  store <16 x i8> <i8 0, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, ptr %5, align 16
  store <16 x i8> <i8 16, i8 17, i8 18, i8 19, i8 20, i8 21, i8 22, i8 23, i8 24, i8 25, i8 26, i8 27, i8 28, i8 29, i8 30, i8 31>, ptr %41, align 16
  %42 = getelementptr inbounds nuw i8, ptr %5, i64 32
  %43 = getelementptr inbounds nuw i8, ptr %5, i64 48
  store <16 x i8> <i8 32, i8 33, i8 34, i8 35, i8 36, i8 37, i8 38, i8 39, i8 40, i8 41, i8 42, i8 43, i8 44, i8 45, i8 46, i8 47>, ptr %42, align 16
  store <16 x i8> <i8 48, i8 49, i8 50, i8 51, i8 52, i8 53, i8 54, i8 55, i8 56, i8 57, i8 58, i8 59, i8 60, i8 61, i8 62, i8 63>, ptr %43, align 16
  %44 = getelementptr inbounds nuw i8, ptr %5, i64 64
  %45 = getelementptr inbounds nuw i8, ptr %5, i64 80
  store <16 x i8> <i8 64, i8 65, i8 66, i8 67, i8 68, i8 69, i8 70, i8 71, i8 72, i8 73, i8 74, i8 75, i8 76, i8 77, i8 78, i8 79>, ptr %44, align 16
  store <16 x i8> <i8 80, i8 81, i8 82, i8 83, i8 84, i8 85, i8 86, i8 87, i8 88, i8 89, i8 90, i8 91, i8 92, i8 93, i8 94, i8 95>, ptr %45, align 16
  %46 = getelementptr inbounds nuw i8, ptr %5, i64 96
  %47 = getelementptr inbounds nuw i8, ptr %5, i64 112
  store <16 x i8> <i8 96, i8 97, i8 98, i8 99, i8 100, i8 101, i8 102, i8 103, i8 104, i8 105, i8 106, i8 107, i8 108, i8 109, i8 110, i8 111>, ptr %46, align 16
  store <16 x i8> <i8 112, i8 113, i8 114, i8 115, i8 116, i8 117, i8 118, i8 119, i8 120, i8 121, i8 122, i8 123, i8 124, i8 125, i8 126, i8 127>, ptr %47, align 16
  %48 = getelementptr inbounds nuw i8, ptr %5, i64 128
  %49 = getelementptr inbounds nuw i8, ptr %5, i64 144
  store <16 x i8> <i8 -128, i8 -127, i8 -126, i8 -125, i8 -124, i8 -123, i8 -122, i8 -121, i8 -120, i8 -119, i8 -118, i8 -117, i8 -116, i8 -115, i8 -114, i8 -113>, ptr %48, align 16
  store <16 x i8> <i8 -112, i8 -111, i8 -110, i8 -109, i8 -108, i8 -107, i8 -106, i8 -105, i8 -104, i8 -103, i8 -102, i8 -101, i8 -100, i8 -99, i8 -98, i8 -97>, ptr %49, align 16
  %50 = getelementptr inbounds nuw i8, ptr %5, i64 160
  %51 = getelementptr inbounds nuw i8, ptr %5, i64 176
  store <16 x i8> <i8 -96, i8 -95, i8 -94, i8 -93, i8 -92, i8 -91, i8 -90, i8 -89, i8 -88, i8 -87, i8 -86, i8 -85, i8 -84, i8 -83, i8 -82, i8 -81>, ptr %50, align 16
  store <16 x i8> <i8 -80, i8 -79, i8 -78, i8 -77, i8 -76, i8 -75, i8 -74, i8 -73, i8 -72, i8 -71, i8 -70, i8 -69, i8 -68, i8 -67, i8 -66, i8 -65>, ptr %51, align 16
  %52 = getelementptr inbounds nuw i8, ptr %5, i64 192
  %53 = getelementptr inbounds nuw i8, ptr %5, i64 208
  store <16 x i8> <i8 -64, i8 -63, i8 -62, i8 -61, i8 -60, i8 -59, i8 -58, i8 -57, i8 -56, i8 -55, i8 -54, i8 -53, i8 -52, i8 -51, i8 -50, i8 -49>, ptr %52, align 16
  store <16 x i8> <i8 -48, i8 -47, i8 -46, i8 -45, i8 -44, i8 -43, i8 -42, i8 -41, i8 -40, i8 -39, i8 -38, i8 -37, i8 -36, i8 -35, i8 -34, i8 -33>, ptr %53, align 16
  %54 = getelementptr inbounds nuw i8, ptr %5, i64 224
  %55 = getelementptr inbounds nuw i8, ptr %5, i64 240
  store <16 x i8> <i8 -32, i8 -31, i8 -30, i8 -29, i8 -28, i8 -27, i8 -26, i8 -25, i8 -24, i8 -23, i8 -22, i8 -21, i8 -20, i8 -19, i8 -18, i8 -17>, ptr %54, align 16
  store <16 x i8> <i8 -16, i8 -15, i8 -14, i8 -13, i8 -12, i8 -11, i8 -10, i8 -9, i8 -8, i8 -7, i8 -6, i8 -5, i8 -4, i8 -3, i8 -2, i8 -1>, ptr %55, align 16
  br label %56

56:                                               ; preds = %37, %56
  %57 = phi i64 [ %74, %56 ], [ 0, %37 ]
  %58 = phi i32 [ %69, %56 ], [ 0, %37 ]
  %59 = getelementptr inbounds nuw i8, ptr %5, i64 %57
  %60 = load i8, ptr %59, align 1
  %61 = zext i8 %60 to i32
  %62 = add i32 %58, %61
  %63 = trunc nuw nsw i64 %57 to i32
  %64 = srem i32 %63, %23
  %65 = zext nneg i32 %64 to i64
  %66 = getelementptr inbounds nuw i8, ptr %19, i64 %65
  %67 = load i8, ptr %66, align 1
  %68 = zext i8 %67 to i32
  %69 = add i32 %62, %68
  %70 = and i32 %69, 255
  %71 = zext nneg i32 %70 to i64
  %72 = getelementptr inbounds nuw i8, ptr %5, i64 %71
  %73 = load i8, ptr %72, align 1
  store i8 %73, ptr %59, align 1
  store i8 %60, ptr %72, align 1
  %74 = add nuw nsw i64 %57, 1
  %75 = icmp eq i64 %74, 256
  br i1 %75, label %76, label %56, !llvm.loop !8

76:                                               ; preds = %56
  %77 = getelementptr inbounds nuw i8, ptr %5, i64 256
  store i8 0, ptr %77, align 16
  %78 = getelementptr inbounds nuw i8, ptr %5, i64 257
  store i8 0, ptr %78, align 1
  call void @llvm.lifetime.start.p0(ptr nonnull %6)
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 1 dereferenceable(258) %6, ptr noundef nonnull align 16 dereferenceable(258) %5, i64 258, i1 false)
  %79 = shl i64 %24, 32
  %80 = ashr exact i64 %79, 32
  %81 = tail call noalias ptr @malloc(i64 noundef %80) #15
  %82 = icmp eq ptr %81, null
  br i1 %82, label %83, label %86

83:                                               ; preds = %76
  %84 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %85 = tail call i64 @fwrite(ptr nonnull @"??_C@_0P@LEGHBOFO@malloc?5failed?6?$AA@", i64 14, i64 1, ptr %84)
  br label %665

86:                                               ; preds = %76
  %87 = icmp sgt i32 %25, 0
  br i1 %87, label %88, label %123

88:                                               ; preds = %86
  %89 = and i64 %24, 2147483647
  br label %90

90:                                               ; preds = %90, %88
  %91 = phi i8 [ 0, %88 ], [ %100, %90 ]
  %92 = phi i8 [ 0, %88 ], [ %96, %90 ]
  %93 = phi i64 [ 0, %88 ], [ %111, %90 ]
  %94 = getelementptr inbounds nuw i8, ptr %21, i64 %93
  %95 = load i8, ptr %94, align 1
  %96 = add i8 %92, 1
  %97 = zext i8 %96 to i64
  %98 = getelementptr inbounds nuw i8, ptr %5, i64 %97
  %99 = load i8, ptr %98, align 1
  %100 = add i8 %91, %99
  %101 = zext i8 %100 to i64
  %102 = getelementptr inbounds nuw i8, ptr %5, i64 %101
  %103 = load i8, ptr %102, align 1
  store i8 %103, ptr %98, align 1
  store i8 %99, ptr %102, align 1
  %104 = load i8, ptr %98, align 1
  %105 = add i8 %104, %99
  %106 = zext i8 %105 to i64
  %107 = getelementptr inbounds nuw i8, ptr %5, i64 %106
  %108 = load i8, ptr %107, align 1
  %109 = xor i8 %108, %95
  %110 = getelementptr inbounds nuw i8, ptr %81, i64 %93
  store i8 %109, ptr %110, align 1
  %111 = add nuw nsw i64 %93, 1
  %112 = icmp eq i64 %111, %89
  br i1 %112, label %113, label %90, !llvm.loop !10

113:                                              ; preds = %90
  store i8 %96, ptr %77, align 16
  store i8 %100, ptr %78, align 1
  %114 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0N@FADNOEAA@Ciphertext?3?5?$AA@")
  br label %115

115:                                              ; preds = %115, %113
  %116 = phi i64 [ 0, %113 ], [ %121, %115 ]
  %117 = getelementptr inbounds nuw i8, ptr %81, i64 %116
  %118 = load i8, ptr %117, align 1
  %119 = zext i8 %118 to i32
  %120 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_04NOJCDH@?$CF02x?$AA@", i32 noundef %119)
  %121 = add nuw nsw i64 %116, 1
  %122 = icmp eq i64 %121, %89
  br i1 %122, label %126, label %115, !llvm.loop !11

123:                                              ; preds = %86
  %124 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0N@FADNOEAA@Ciphertext?3?5?$AA@")
  %125 = tail call i32 @putchar(i32 10)
  br label %179

126:                                              ; preds = %115
  %127 = tail call i32 @putchar(i32 10)
  %128 = and i64 %24, 1
  %129 = icmp eq i64 %89, 1
  br i1 %129, label %164, label %130

130:                                              ; preds = %126
  %131 = and i64 %24, 2147483646
  br label %132

132:                                              ; preds = %132, %130
  %133 = phi i64 [ 0, %130 ], [ %159, %132 ]
  %134 = phi i32 [ 0, %130 ], [ %158, %132 ]
  %135 = phi i64 [ 0, %130 ], [ %160, %132 ]
  %136 = getelementptr inbounds nuw i8, ptr %81, i64 %133
  %137 = load i8, ptr %136, align 1
  %138 = zext i8 %137 to i32
  %139 = trunc i64 %133 to i32
  %140 = mul i32 %139, 37
  %141 = xor i32 %140, %138
  %142 = tail call i32 @llvm.fshl.i32(i32 %141, i32 %141, i32 5)
  %143 = xor i32 %142, %134
  %144 = mul i32 %142, -1640531535
  %145 = add i32 %143, %144
  %146 = tail call i32 @llvm.fshl.i32(i32 %145, i32 %145, i32 7)
  %147 = or disjoint i64 %133, 1
  %148 = getelementptr inbounds nuw i8, ptr %81, i64 %147
  %149 = load i8, ptr %148, align 1
  %150 = zext i8 %149 to i32
  %151 = trunc i64 %147 to i32
  %152 = mul i32 %151, 37
  %153 = xor i32 %152, %150
  %154 = tail call i32 @llvm.fshl.i32(i32 %153, i32 %153, i32 5)
  %155 = xor i32 %154, %146
  %156 = mul i32 %154, -1640531535
  %157 = add i32 %155, %156
  %158 = tail call i32 @llvm.fshl.i32(i32 %157, i32 %157, i32 7)
  %159 = add nuw nsw i64 %133, 2
  %160 = add i64 %135, 2
  %161 = icmp eq i64 %160, %131
  br i1 %161, label %162, label %132, !llvm.loop !12

162:                                              ; preds = %132
  %163 = icmp eq i64 %128, 0
  br i1 %163, label %179, label %164

164:                                              ; preds = %162, %126
  %165 = phi i64 [ 0, %126 ], [ %159, %162 ]
  %166 = phi i32 [ 0, %126 ], [ %158, %162 ]
  %167 = icmp ne i64 %128, 0
  tail call void @llvm.assume(i1 %167)
  %168 = getelementptr inbounds nuw i8, ptr %81, i64 %165
  %169 = load i8, ptr %168, align 1
  %170 = zext i8 %169 to i32
  %171 = trunc i64 %165 to i32
  %172 = mul i32 %171, 37
  %173 = xor i32 %172, %170
  %174 = tail call i32 @llvm.fshl.i32(i32 %173, i32 %173, i32 5)
  %175 = xor i32 %174, %166
  %176 = mul i32 %174, -1640531535
  %177 = add i32 %175, %176
  %178 = tail call i32 @llvm.fshl.i32(i32 %177, i32 %177, i32 7)
  br label %179

179:                                              ; preds = %164, %162, %123
  %180 = phi i32 [ 0, %123 ], [ %158, %162 ], [ %178, %164 ]
  %181 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BD@GHNLPNIN@CT?5checksum?3?5?$CF08x?6?$AA@", i32 noundef %180)
  call void @llvm.lifetime.start.p0(ptr nonnull %7) #14
  %182 = getelementptr inbounds nuw i8, ptr %7, i64 16
  store <16 x i8> <i8 0, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, ptr %7, align 16
  store <16 x i8> <i8 16, i8 17, i8 18, i8 19, i8 20, i8 21, i8 22, i8 23, i8 24, i8 25, i8 26, i8 27, i8 28, i8 29, i8 30, i8 31>, ptr %182, align 16
  %183 = getelementptr inbounds nuw i8, ptr %7, i64 32
  %184 = getelementptr inbounds nuw i8, ptr %7, i64 48
  store <16 x i8> <i8 32, i8 33, i8 34, i8 35, i8 36, i8 37, i8 38, i8 39, i8 40, i8 41, i8 42, i8 43, i8 44, i8 45, i8 46, i8 47>, ptr %183, align 16
  store <16 x i8> <i8 48, i8 49, i8 50, i8 51, i8 52, i8 53, i8 54, i8 55, i8 56, i8 57, i8 58, i8 59, i8 60, i8 61, i8 62, i8 63>, ptr %184, align 16
  %185 = getelementptr inbounds nuw i8, ptr %7, i64 64
  %186 = getelementptr inbounds nuw i8, ptr %7, i64 80
  store <16 x i8> <i8 64, i8 65, i8 66, i8 67, i8 68, i8 69, i8 70, i8 71, i8 72, i8 73, i8 74, i8 75, i8 76, i8 77, i8 78, i8 79>, ptr %185, align 16
  store <16 x i8> <i8 80, i8 81, i8 82, i8 83, i8 84, i8 85, i8 86, i8 87, i8 88, i8 89, i8 90, i8 91, i8 92, i8 93, i8 94, i8 95>, ptr %186, align 16
  %187 = getelementptr inbounds nuw i8, ptr %7, i64 96
  %188 = getelementptr inbounds nuw i8, ptr %7, i64 112
  store <16 x i8> <i8 96, i8 97, i8 98, i8 99, i8 100, i8 101, i8 102, i8 103, i8 104, i8 105, i8 106, i8 107, i8 108, i8 109, i8 110, i8 111>, ptr %187, align 16
  store <16 x i8> <i8 112, i8 113, i8 114, i8 115, i8 116, i8 117, i8 118, i8 119, i8 120, i8 121, i8 122, i8 123, i8 124, i8 125, i8 126, i8 127>, ptr %188, align 16
  %189 = getelementptr inbounds nuw i8, ptr %7, i64 128
  %190 = getelementptr inbounds nuw i8, ptr %7, i64 144
  store <16 x i8> <i8 -128, i8 -127, i8 -126, i8 -125, i8 -124, i8 -123, i8 -122, i8 -121, i8 -120, i8 -119, i8 -118, i8 -117, i8 -116, i8 -115, i8 -114, i8 -113>, ptr %189, align 16
  store <16 x i8> <i8 -112, i8 -111, i8 -110, i8 -109, i8 -108, i8 -107, i8 -106, i8 -105, i8 -104, i8 -103, i8 -102, i8 -101, i8 -100, i8 -99, i8 -98, i8 -97>, ptr %190, align 16
  %191 = getelementptr inbounds nuw i8, ptr %7, i64 160
  %192 = getelementptr inbounds nuw i8, ptr %7, i64 176
  store <16 x i8> <i8 -96, i8 -95, i8 -94, i8 -93, i8 -92, i8 -91, i8 -90, i8 -89, i8 -88, i8 -87, i8 -86, i8 -85, i8 -84, i8 -83, i8 -82, i8 -81>, ptr %191, align 16
  store <16 x i8> <i8 -80, i8 -79, i8 -78, i8 -77, i8 -76, i8 -75, i8 -74, i8 -73, i8 -72, i8 -71, i8 -70, i8 -69, i8 -68, i8 -67, i8 -66, i8 -65>, ptr %192, align 16
  %193 = getelementptr inbounds nuw i8, ptr %7, i64 192
  %194 = getelementptr inbounds nuw i8, ptr %7, i64 208
  store <16 x i8> <i8 -64, i8 -63, i8 -62, i8 -61, i8 -60, i8 -59, i8 -58, i8 -57, i8 -56, i8 -55, i8 -54, i8 -53, i8 -52, i8 -51, i8 -50, i8 -49>, ptr %193, align 16
  store <16 x i8> <i8 -48, i8 -47, i8 -46, i8 -45, i8 -44, i8 -43, i8 -42, i8 -41, i8 -40, i8 -39, i8 -38, i8 -37, i8 -36, i8 -35, i8 -34, i8 -33>, ptr %194, align 16
  %195 = getelementptr inbounds nuw i8, ptr %7, i64 224
  %196 = getelementptr inbounds nuw i8, ptr %7, i64 240
  store <16 x i8> <i8 -32, i8 -31, i8 -30, i8 -29, i8 -28, i8 -27, i8 -26, i8 -25, i8 -24, i8 -23, i8 -22, i8 -21, i8 -20, i8 -19, i8 -18, i8 -17>, ptr %195, align 16
  store <16 x i8> <i8 -16, i8 -15, i8 -14, i8 -13, i8 -12, i8 -11, i8 -10, i8 -9, i8 -8, i8 -7, i8 -6, i8 -5, i8 -4, i8 -3, i8 -2, i8 -1>, ptr %196, align 16
  br label %197

197:                                              ; preds = %179, %197
  %198 = phi i64 [ %215, %197 ], [ 0, %179 ]
  %199 = phi i32 [ %210, %197 ], [ 0, %179 ]
  %200 = getelementptr inbounds nuw i8, ptr %7, i64 %198
  %201 = load i8, ptr %200, align 1
  %202 = zext i8 %201 to i32
  %203 = add i32 %199, %202
  %204 = trunc nuw nsw i64 %198 to i32
  %205 = srem i32 %204, %23
  %206 = zext nneg i32 %205 to i64
  %207 = getelementptr inbounds nuw i8, ptr %19, i64 %206
  %208 = load i8, ptr %207, align 1
  %209 = zext i8 %208 to i32
  %210 = add i32 %203, %209
  %211 = and i32 %210, 255
  %212 = zext nneg i32 %211 to i64
  %213 = getelementptr inbounds nuw i8, ptr %7, i64 %212
  %214 = load i8, ptr %213, align 1
  store i8 %214, ptr %200, align 1
  store i8 %201, ptr %213, align 1
  %215 = add nuw nsw i64 %198, 1
  %216 = icmp eq i64 %215, 256
  br i1 %216, label %217, label %197, !llvm.loop !8

217:                                              ; preds = %197
  %218 = getelementptr inbounds nuw i8, ptr %7, i64 256
  store i8 0, ptr %218, align 16
  %219 = getelementptr inbounds nuw i8, ptr %7, i64 257
  store i8 0, ptr %219, align 1
  %220 = add nsw i64 %80, 1
  %221 = tail call noalias ptr @malloc(i64 noundef %220) #15
  %222 = icmp eq ptr %221, null
  br i1 %222, label %223, label %224

223:                                              ; preds = %217
  tail call void @free(ptr noundef nonnull %81)
  br label %663

224:                                              ; preds = %217
  br i1 %87, label %225, label %251

225:                                              ; preds = %224
  %226 = and i64 %24, 2147483647
  br label %227

227:                                              ; preds = %227, %225
  %228 = phi i8 [ 0, %225 ], [ %237, %227 ]
  %229 = phi i8 [ 0, %225 ], [ %233, %227 ]
  %230 = phi i64 [ 0, %225 ], [ %248, %227 ]
  %231 = getelementptr inbounds nuw i8, ptr %81, i64 %230
  %232 = load i8, ptr %231, align 1
  %233 = add i8 %229, 1
  %234 = zext i8 %233 to i64
  %235 = getelementptr inbounds nuw i8, ptr %7, i64 %234
  %236 = load i8, ptr %235, align 1
  %237 = add i8 %228, %236
  %238 = zext i8 %237 to i64
  %239 = getelementptr inbounds nuw i8, ptr %7, i64 %238
  %240 = load i8, ptr %239, align 1
  store i8 %240, ptr %235, align 1
  store i8 %236, ptr %239, align 1
  %241 = load i8, ptr %235, align 1
  %242 = add i8 %241, %236
  %243 = zext i8 %242 to i64
  %244 = getelementptr inbounds nuw i8, ptr %7, i64 %243
  %245 = load i8, ptr %244, align 1
  %246 = xor i8 %245, %232
  %247 = getelementptr inbounds nuw i8, ptr %221, i64 %230
  store i8 %246, ptr %247, align 1
  %248 = add nuw nsw i64 %230, 1
  %249 = icmp eq i64 %248, %226
  br i1 %249, label %250, label %227, !llvm.loop !10

250:                                              ; preds = %227
  store i8 %233, ptr %218, align 16
  store i8 %237, ptr %219, align 1
  br label %251

251:                                              ; preds = %250, %224
  %252 = getelementptr inbounds i8, ptr %221, i64 %80
  store i8 0, ptr %252, align 1
  %253 = tail call i32 @memcmp(ptr noundef nonnull %21, ptr noundef nonnull %221, i64 noundef %80)
  %254 = icmp eq i32 %253, 0
  br i1 %254, label %262, label %255

255:                                              ; preds = %251
  %256 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %257 = tail call i64 @fwrite(ptr nonnull @"??_C@_0CP@LEKLIDOD@FAIL?3?5Decrypted?5text?5does?5not?5ma@", i64 46, i64 1, ptr %256)
  %258 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %259 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef %258, ptr noundef nonnull @"??_C@_0BA@IDGDPNHB@?5?5Expected?3?5?$CFs?6?$AA@", ptr noundef nonnull %21) #14
  %260 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %261 = tail call i32 (ptr, ptr, ...) @fprintf(ptr noundef %260, ptr noundef nonnull @"??_C@_0BA@GKHOJLAE@?5?5Got?3?5?5?5?5?5?5?$CFs?6?$AA@", ptr noundef nonnull %221) #14
  br label %262

262:                                              ; preds = %255, %251
  %263 = phi i32 [ 1, %255 ], [ 0, %251 ]
  call void @llvm.lifetime.start.p0(ptr nonnull %8) #14
  call void @llvm.lifetime.start.p0(ptr nonnull %9) #14
  %264 = tail call i32 @llvm.smin.i32(i32 %25, i32 255)
  br i1 %87, label %265, label %392

265:                                              ; preds = %262
  %266 = zext nneg i32 %264 to i64
  %267 = icmp slt i32 %25, 4
  br i1 %267, label %314, label %268

268:                                              ; preds = %265
  %269 = icmp slt i32 %25, 16
  br i1 %269, label %291, label %270

270:                                              ; preds = %268
  %271 = and i64 %266, 12
  %272 = and i64 %266, 240
  br label %273

273:                                              ; preds = %273, %270
  %274 = phi i64 [ 0, %270 ], [ %284, %273 ]
  %275 = phi <16 x i8> [ <i8 0, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, %270 ], [ %285, %273 ]
  %276 = mul <16 x i8> %275, splat (i8 31)
  %277 = add <16 x i8> %276, splat (i8 17)
  %278 = xor <16 x i8> %277, splat (i8 -85)
  %279 = getelementptr inbounds nuw i8, ptr %21, i64 %274
  %280 = load <16 x i8>, ptr %279, align 1
  %281 = tail call <16 x i8> @llvm.fshl.v16i8(<16 x i8> %278, <16 x i8> %278, <16 x i8> splat (i8 3))
  %282 = xor <16 x i8> %281, %280
  %283 = getelementptr inbounds nuw i8, ptr %8, i64 %274
  store <16 x i8> %282, ptr %283, align 16
  %284 = add nuw i64 %274, 16
  %285 = add <16 x i8> %275, splat (i8 16)
  %286 = icmp eq i64 %284, %272
  br i1 %286, label %287, label %273, !llvm.loop !13

287:                                              ; preds = %273
  %288 = icmp eq i64 %272, %266
  br i1 %288, label %329, label %289

289:                                              ; preds = %287
  %290 = icmp eq i64 %271, 0
  br i1 %290, label %314, label %291, !prof !16

291:                                              ; preds = %268, %289
  %292 = phi i64 [ %272, %289 ], [ 0, %268 ]
  %293 = and i64 %266, 252
  %294 = trunc nuw i64 %292 to i8
  %295 = insertelement <4 x i8> poison, i8 %294, i64 0
  %296 = shufflevector <4 x i8> %295, <4 x i8> poison, <4 x i32> zeroinitializer
  %297 = or disjoint <4 x i8> %296, <i8 0, i8 1, i8 2, i8 3>
  br label %298

298:                                              ; preds = %298, %291
  %299 = phi i64 [ %292, %291 ], [ %309, %298 ]
  %300 = phi <4 x i8> [ %297, %291 ], [ %310, %298 ]
  %301 = mul <4 x i8> %300, splat (i8 31)
  %302 = add <4 x i8> %301, splat (i8 17)
  %303 = xor <4 x i8> %302, splat (i8 -85)
  %304 = getelementptr inbounds nuw i8, ptr %21, i64 %299
  %305 = load <4 x i8>, ptr %304, align 1
  %306 = tail call <4 x i8> @llvm.fshl.v4i8(<4 x i8> %303, <4 x i8> %303, <4 x i8> splat (i8 3))
  %307 = xor <4 x i8> %306, %305
  %308 = getelementptr inbounds nuw i8, ptr %8, i64 %299
  store <4 x i8> %307, ptr %308, align 4
  %309 = add nuw i64 %299, 4
  %310 = add <4 x i8> %300, splat (i8 4)
  %311 = icmp eq i64 %309, %293
  br i1 %311, label %312, label %298, !llvm.loop !17

312:                                              ; preds = %298
  %313 = icmp eq i64 %293, %266
  br i1 %313, label %329, label %314

314:                                              ; preds = %265, %289, %312
  %315 = phi i64 [ 0, %265 ], [ %272, %289 ], [ %293, %312 ]
  br label %316

316:                                              ; preds = %314, %316
  %317 = phi i64 [ %327, %316 ], [ %315, %314 ]
  %318 = trunc i64 %317 to i8
  %319 = mul i8 %318, 31
  %320 = add i8 %319, 17
  %321 = xor i8 %320, -85
  %322 = getelementptr inbounds nuw i8, ptr %21, i64 %317
  %323 = load i8, ptr %322, align 1
  %324 = tail call i8 @llvm.fshl.i8(i8 %321, i8 %321, i8 3)
  %325 = xor i8 %324, %323
  %326 = getelementptr inbounds nuw i8, ptr %8, i64 %317
  store i8 %325, ptr %326, align 1
  %327 = add nuw nsw i64 %317, 1
  %328 = icmp eq i64 %327, %266
  br i1 %328, label %329, label %316, !llvm.loop !18

329:                                              ; preds = %316, %312, %287
  %330 = icmp slt i32 %25, 4
  br i1 %330, label %377, label %331

331:                                              ; preds = %329
  %332 = icmp slt i32 %25, 16
  br i1 %332, label %354, label %333

333:                                              ; preds = %331
  %334 = and i64 %266, 12
  %335 = and i64 %266, 240
  br label %336

336:                                              ; preds = %336, %333
  %337 = phi i64 [ 0, %333 ], [ %347, %336 ]
  %338 = phi <16 x i8> [ <i8 0, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, %333 ], [ %348, %336 ]
  %339 = mul <16 x i8> %338, splat (i8 31)
  %340 = add <16 x i8> %339, splat (i8 17)
  %341 = xor <16 x i8> %340, splat (i8 -85)
  %342 = getelementptr inbounds nuw i8, ptr %8, i64 %337
  %343 = load <16 x i8>, ptr %342, align 16
  %344 = tail call <16 x i8> @llvm.fshl.v16i8(<16 x i8> %341, <16 x i8> %341, <16 x i8> splat (i8 3))
  %345 = xor <16 x i8> %344, %343
  %346 = getelementptr inbounds nuw i8, ptr %9, i64 %337
  store <16 x i8> %345, ptr %346, align 16
  %347 = add nuw i64 %337, 16
  %348 = add <16 x i8> %338, splat (i8 16)
  %349 = icmp eq i64 %347, %335
  br i1 %349, label %350, label %336, !llvm.loop !19

350:                                              ; preds = %336
  %351 = icmp eq i64 %335, %266
  br i1 %351, label %392, label %352

352:                                              ; preds = %350
  %353 = icmp eq i64 %334, 0
  br i1 %353, label %377, label %354, !prof !16

354:                                              ; preds = %331, %352
  %355 = phi i64 [ %335, %352 ], [ 0, %331 ]
  %356 = and i64 %266, 252
  %357 = trunc nuw i64 %355 to i8
  %358 = insertelement <4 x i8> poison, i8 %357, i64 0
  %359 = shufflevector <4 x i8> %358, <4 x i8> poison, <4 x i32> zeroinitializer
  %360 = or disjoint <4 x i8> %359, <i8 0, i8 1, i8 2, i8 3>
  br label %361

361:                                              ; preds = %361, %354
  %362 = phi i64 [ %355, %354 ], [ %372, %361 ]
  %363 = phi <4 x i8> [ %360, %354 ], [ %373, %361 ]
  %364 = mul <4 x i8> %363, splat (i8 31)
  %365 = add <4 x i8> %364, splat (i8 17)
  %366 = xor <4 x i8> %365, splat (i8 -85)
  %367 = getelementptr inbounds nuw i8, ptr %8, i64 %362
  %368 = load <4 x i8>, ptr %367, align 4
  %369 = tail call <4 x i8> @llvm.fshl.v4i8(<4 x i8> %366, <4 x i8> %366, <4 x i8> splat (i8 3))
  %370 = xor <4 x i8> %369, %368
  %371 = getelementptr inbounds nuw i8, ptr %9, i64 %362
  store <4 x i8> %370, ptr %371, align 4
  %372 = add nuw i64 %362, 4
  %373 = add <4 x i8> %363, splat (i8 4)
  %374 = icmp eq i64 %372, %356
  br i1 %374, label %375, label %361, !llvm.loop !20

375:                                              ; preds = %361
  %376 = icmp eq i64 %356, %266
  br i1 %376, label %392, label %377

377:                                              ; preds = %329, %352, %375
  %378 = phi i64 [ 0, %329 ], [ %335, %352 ], [ %356, %375 ]
  br label %379

379:                                              ; preds = %377, %379
  %380 = phi i64 [ %390, %379 ], [ %378, %377 ]
  %381 = trunc i64 %380 to i8
  %382 = mul i8 %381, 31
  %383 = add i8 %382, 17
  %384 = xor i8 %383, -85
  %385 = getelementptr inbounds nuw i8, ptr %8, i64 %380
  %386 = load i8, ptr %385, align 1
  %387 = tail call i8 @llvm.fshl.i8(i8 %384, i8 %384, i8 3)
  %388 = xor i8 %387, %386
  %389 = getelementptr inbounds nuw i8, ptr %9, i64 %380
  store i8 %388, ptr %389, align 1
  %390 = add nuw nsw i64 %380, 1
  %391 = icmp eq i64 %390, %266
  br i1 %391, label %392, label %379, !llvm.loop !21

392:                                              ; preds = %379, %350, %375, %262
  %393 = sext i32 %264 to i64
  %394 = getelementptr inbounds i8, ptr %9, i64 %393
  store i8 0, ptr %394, align 1
  %395 = call i32 @memcmp(ptr noundef nonnull %21, ptr noundef nonnull %9, i64 noundef %393)
  %396 = icmp eq i32 %395, 0
  br i1 %396, label %400, label %397

397:                                              ; preds = %392
  %398 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %399 = tail call i64 @fwrite(ptr nonnull @"??_C@_0DB@KEHBAICJ@FAIL?3?5XOR?5string?5encode?1decode?5r@", i64 48, i64 1, ptr %398)
  br label %400

400:                                              ; preds = %397, %392
  %401 = phi i32 [ 1, %397 ], [ %263, %392 ]
  %402 = icmp sgt i32 %25, 63
  br i1 %402, label %403, label %502

403:                                              ; preds = %400
  call void @llvm.lifetime.start.p0(ptr nonnull %3) #14
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(1024) %3, i8 0, i64 1024, i1 false)
  call void @llvm.lifetime.start.p0(ptr nonnull %4) #14
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 1 dereferenceable(258) %4, ptr noundef nonnull readonly align 1 dereferenceable(258) %6, i64 258, i1 false)
  %404 = getelementptr inbounds nuw i8, ptr %4, i64 256
  %405 = getelementptr inbounds nuw i8, ptr %4, i64 257
  %406 = load i8, ptr %404, align 1
  %407 = load i8, ptr %405, align 1
  br label %411

408:                                              ; preds = %411
  %409 = uitofp nneg i32 %25 to double
  %410 = fmul double %409, 3.906250e-03
  br label %466

411:                                              ; preds = %411, %403
  %412 = phi i32 [ 0, %403 ], [ %442, %411 ]
  %413 = phi i32 [ 0, %403 ], [ %445, %411 ]
  %414 = phi i32 [ 0, %403 ], [ %448, %411 ]
  %415 = phi i32 [ 0, %403 ], [ %451, %411 ]
  %416 = phi i32 [ 0, %403 ], [ %454, %411 ]
  %417 = phi i32 [ 0, %403 ], [ %457, %411 ]
  %418 = phi i32 [ 0, %403 ], [ %460, %411 ]
  %419 = phi i32 [ 0, %403 ], [ %463, %411 ]
  %420 = phi i32 [ 0, %403 ], [ %464, %411 ]
  %421 = phi i8 [ %406, %403 ], [ %423, %411 ]
  %422 = phi i8 [ %407, %403 ], [ %427, %411 ]
  %423 = add i8 %421, 1
  %424 = zext i8 %423 to i64
  %425 = getelementptr inbounds nuw i8, ptr %4, i64 %424
  %426 = load i8, ptr %425, align 1
  %427 = add i8 %426, %422
  %428 = zext i8 %427 to i64
  %429 = getelementptr inbounds nuw i8, ptr %4, i64 %428
  %430 = load i8, ptr %429, align 1
  store i8 %430, ptr %425, align 1
  store i8 %426, ptr %429, align 1
  %431 = load i8, ptr %425, align 1
  %432 = add i8 %431, %426
  %433 = zext i8 %432 to i64
  %434 = getelementptr inbounds nuw i8, ptr %4, i64 %433
  %435 = load i8, ptr %434, align 1
  %436 = zext i8 %435 to i64
  %437 = getelementptr inbounds nuw i32, ptr %3, i64 %436
  %438 = load i32, ptr %437, align 4
  %439 = add i32 %438, 1
  store i32 %439, ptr %437, align 4
  %440 = zext i8 %435 to i32
  %441 = and i32 %440, 1
  %442 = add i32 %441, %412
  %443 = lshr i32 %440, 1
  %444 = and i32 %443, 1
  %445 = add i32 %444, %413
  %446 = lshr i32 %440, 2
  %447 = and i32 %446, 1
  %448 = add i32 %447, %414
  %449 = lshr i32 %440, 3
  %450 = and i32 %449, 1
  %451 = add i32 %450, %415
  %452 = lshr i32 %440, 4
  %453 = and i32 %452, 1
  %454 = add i32 %453, %416
  %455 = lshr i32 %440, 5
  %456 = and i32 %455, 1
  %457 = add i32 %456, %417
  %458 = lshr i32 %440, 6
  %459 = and i32 %458, 1
  %460 = add i32 %459, %418
  %461 = lshr i8 %435, 7
  %462 = zext nneg i8 %461 to i32
  %463 = add i32 %419, %462
  %464 = add nuw nsw i32 %420, 1
  %465 = icmp eq i32 %464, %25
  br i1 %465, label %408, label %411, !llvm.loop !22

466:                                              ; preds = %466, %408
  %467 = phi i64 [ 0, %408 ], [ %484, %466 ]
  %468 = phi double [ 0.000000e+00, %408 ], [ %483, %466 ]
  %469 = getelementptr inbounds nuw i32, ptr %3, i64 %467
  %470 = load i32, ptr %469, align 8
  %471 = uitofp i32 %470 to double
  %472 = fsub double %471, %410
  %473 = fmul double %472, %472
  %474 = fdiv double %473, %410
  %475 = fadd double %468, %474
  %476 = getelementptr inbounds nuw i32, ptr %3, i64 %467
  %477 = getelementptr inbounds nuw i8, ptr %476, i64 4
  %478 = load i32, ptr %477, align 4
  %479 = uitofp i32 %478 to double
  %480 = fsub double %479, %410
  %481 = fmul double %480, %480
  %482 = fdiv double %481, %410
  %483 = fadd double %475, %482
  %484 = add nuw nsw i64 %467, 2
  %485 = icmp eq i64 %484, 256
  br i1 %485, label %486, label %466, !llvm.loop !23

486:                                              ; preds = %466
  %487 = add i32 %463, %442
  %488 = add i32 %487, %445
  %489 = add i32 %488, %448
  %490 = add i32 %489, %451
  %491 = add i32 %490, %454
  %492 = add i32 %491, %457
  %493 = add i32 %492, %460
  %494 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0CC@JCHMFFP@?5?5Keystream?5analysis?5?$CI?$CFd?5bytes?$CJ?3@", i32 noundef range(i32 64, -2147483648) %25)
  %495 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0DB@MCFGGKFC@?5?5Chi?9squared?3?5?$CF?42f?5?$CIexpected?5?$HO2@", double noundef %483)
  %496 = uitofp i32 %493 to double
  %497 = fmul double %496, 1.000000e+02
  %498 = shl nsw i32 %25, 3
  %499 = uitofp nneg i32 %498 to double
  %500 = fdiv double %497, %499
  %501 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0CF@BDAJAHGO@?5?5Total?5ones?3?5?$CFu?5?1?5?$CFu?5bits?5?$CI?$CF?41f@", i32 noundef %493, i32 noundef %498, double noundef %500)
  call void @llvm.lifetime.end.p0(ptr nonnull %4) #14
  call void @llvm.lifetime.end.p0(ptr nonnull %3) #14
  br label %502

502:                                              ; preds = %486, %400
  %503 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  %504 = tail call noalias ptr @malloc(i64 noundef %80) #15
  %505 = tail call noalias ptr @malloc(i64 noundef %80) #15
  %506 = icmp ne ptr %504, null
  %507 = icmp ne ptr %505, null
  %508 = and i1 %506, %507
  br i1 %508, label %509, label %656

509:                                              ; preds = %502
  tail call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 1 %504, ptr nonnull align 1 %21, i64 %80, i1 false)
  %510 = getelementptr inbounds nuw i8, ptr %10, i64 256
  %511 = getelementptr inbounds nuw i8, ptr %10, i64 257
  %512 = and i64 %24, 2147483647
  %513 = getelementptr inbounds nuw i8, ptr %10, i64 16
  %514 = getelementptr inbounds nuw i8, ptr %10, i64 32
  %515 = getelementptr inbounds nuw i8, ptr %10, i64 48
  %516 = getelementptr inbounds nuw i8, ptr %10, i64 64
  %517 = getelementptr inbounds nuw i8, ptr %10, i64 80
  %518 = getelementptr inbounds nuw i8, ptr %10, i64 96
  %519 = getelementptr inbounds nuw i8, ptr %10, i64 112
  %520 = getelementptr inbounds nuw i8, ptr %10, i64 128
  %521 = getelementptr inbounds nuw i8, ptr %10, i64 144
  %522 = getelementptr inbounds nuw i8, ptr %10, i64 160
  %523 = getelementptr inbounds nuw i8, ptr %10, i64 176
  %524 = getelementptr inbounds nuw i8, ptr %10, i64 192
  %525 = getelementptr inbounds nuw i8, ptr %10, i64 208
  %526 = getelementptr inbounds nuw i8, ptr %10, i64 224
  %527 = getelementptr inbounds nuw i8, ptr %10, i64 240
  br label %546

528:                                              ; preds = %594
  %529 = getelementptr inbounds nuw i8, ptr %11, i64 256
  %530 = getelementptr inbounds nuw i8, ptr %11, i64 257
  %531 = getelementptr inbounds nuw i8, ptr %11, i64 16
  %532 = getelementptr inbounds nuw i8, ptr %11, i64 32
  %533 = getelementptr inbounds nuw i8, ptr %11, i64 48
  %534 = getelementptr inbounds nuw i8, ptr %11, i64 64
  %535 = getelementptr inbounds nuw i8, ptr %11, i64 80
  %536 = getelementptr inbounds nuw i8, ptr %11, i64 96
  %537 = getelementptr inbounds nuw i8, ptr %11, i64 112
  %538 = getelementptr inbounds nuw i8, ptr %11, i64 128
  %539 = getelementptr inbounds nuw i8, ptr %11, i64 144
  %540 = getelementptr inbounds nuw i8, ptr %11, i64 160
  %541 = getelementptr inbounds nuw i8, ptr %11, i64 176
  %542 = getelementptr inbounds nuw i8, ptr %11, i64 192
  %543 = getelementptr inbounds nuw i8, ptr %11, i64 208
  %544 = getelementptr inbounds nuw i8, ptr %11, i64 224
  %545 = getelementptr inbounds nuw i8, ptr %11, i64 240
  br label %600

546:                                              ; preds = %509, %594
  %547 = phi i32 [ 0, %509 ], [ %595, %594 ]
  %548 = phi ptr [ %505, %509 ], [ %549, %594 ]
  %549 = phi ptr [ %504, %509 ], [ %548, %594 ]
  call void @llvm.lifetime.start.p0(ptr nonnull %10) #14
  store <16 x i8> <i8 0, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, ptr %10, align 16
  store <16 x i8> <i8 16, i8 17, i8 18, i8 19, i8 20, i8 21, i8 22, i8 23, i8 24, i8 25, i8 26, i8 27, i8 28, i8 29, i8 30, i8 31>, ptr %513, align 16
  store <16 x i8> <i8 32, i8 33, i8 34, i8 35, i8 36, i8 37, i8 38, i8 39, i8 40, i8 41, i8 42, i8 43, i8 44, i8 45, i8 46, i8 47>, ptr %514, align 16
  store <16 x i8> <i8 48, i8 49, i8 50, i8 51, i8 52, i8 53, i8 54, i8 55, i8 56, i8 57, i8 58, i8 59, i8 60, i8 61, i8 62, i8 63>, ptr %515, align 16
  store <16 x i8> <i8 64, i8 65, i8 66, i8 67, i8 68, i8 69, i8 70, i8 71, i8 72, i8 73, i8 74, i8 75, i8 76, i8 77, i8 78, i8 79>, ptr %516, align 16
  store <16 x i8> <i8 80, i8 81, i8 82, i8 83, i8 84, i8 85, i8 86, i8 87, i8 88, i8 89, i8 90, i8 91, i8 92, i8 93, i8 94, i8 95>, ptr %517, align 16
  store <16 x i8> <i8 96, i8 97, i8 98, i8 99, i8 100, i8 101, i8 102, i8 103, i8 104, i8 105, i8 106, i8 107, i8 108, i8 109, i8 110, i8 111>, ptr %518, align 16
  store <16 x i8> <i8 112, i8 113, i8 114, i8 115, i8 116, i8 117, i8 118, i8 119, i8 120, i8 121, i8 122, i8 123, i8 124, i8 125, i8 126, i8 127>, ptr %519, align 16
  store <16 x i8> <i8 -128, i8 -127, i8 -126, i8 -125, i8 -124, i8 -123, i8 -122, i8 -121, i8 -120, i8 -119, i8 -118, i8 -117, i8 -116, i8 -115, i8 -114, i8 -113>, ptr %520, align 16
  store <16 x i8> <i8 -112, i8 -111, i8 -110, i8 -109, i8 -108, i8 -107, i8 -106, i8 -105, i8 -104, i8 -103, i8 -102, i8 -101, i8 -100, i8 -99, i8 -98, i8 -97>, ptr %521, align 16
  store <16 x i8> <i8 -96, i8 -95, i8 -94, i8 -93, i8 -92, i8 -91, i8 -90, i8 -89, i8 -88, i8 -87, i8 -86, i8 -85, i8 -84, i8 -83, i8 -82, i8 -81>, ptr %522, align 16
  store <16 x i8> <i8 -80, i8 -79, i8 -78, i8 -77, i8 -76, i8 -75, i8 -74, i8 -73, i8 -72, i8 -71, i8 -70, i8 -69, i8 -68, i8 -67, i8 -66, i8 -65>, ptr %523, align 16
  store <16 x i8> <i8 -64, i8 -63, i8 -62, i8 -61, i8 -60, i8 -59, i8 -58, i8 -57, i8 -56, i8 -55, i8 -54, i8 -53, i8 -52, i8 -51, i8 -50, i8 -49>, ptr %524, align 16
  store <16 x i8> <i8 -48, i8 -47, i8 -46, i8 -45, i8 -44, i8 -43, i8 -42, i8 -41, i8 -40, i8 -39, i8 -38, i8 -37, i8 -36, i8 -35, i8 -34, i8 -33>, ptr %525, align 16
  store <16 x i8> <i8 -32, i8 -31, i8 -30, i8 -29, i8 -28, i8 -27, i8 -26, i8 -25, i8 -24, i8 -23, i8 -22, i8 -21, i8 -20, i8 -19, i8 -18, i8 -17>, ptr %526, align 16
  store <16 x i8> <i8 -16, i8 -15, i8 -14, i8 -13, i8 -12, i8 -11, i8 -10, i8 -9, i8 -8, i8 -7, i8 -6, i8 -5, i8 -4, i8 -3, i8 -2, i8 -1>, ptr %527, align 16
  br label %550

550:                                              ; preds = %546, %550
  %551 = phi i64 [ %568, %550 ], [ 0, %546 ]
  %552 = phi i32 [ %563, %550 ], [ 0, %546 ]
  %553 = getelementptr inbounds nuw i8, ptr %10, i64 %551
  %554 = load i8, ptr %553, align 1
  %555 = zext i8 %554 to i32
  %556 = add i32 %552, %555
  %557 = trunc nuw nsw i64 %551 to i32
  %558 = srem i32 %557, %23
  %559 = zext nneg i32 %558 to i64
  %560 = getelementptr inbounds nuw i8, ptr %19, i64 %559
  %561 = load i8, ptr %560, align 1
  %562 = zext i8 %561 to i32
  %563 = add i32 %556, %562
  %564 = and i32 %563, 255
  %565 = zext nneg i32 %564 to i64
  %566 = getelementptr inbounds nuw i8, ptr %10, i64 %565
  %567 = load i8, ptr %566, align 1
  store i8 %567, ptr %553, align 1
  store i8 %554, ptr %566, align 1
  %568 = add nuw nsw i64 %551, 1
  %569 = icmp eq i64 %568, 256
  br i1 %569, label %570, label %550, !llvm.loop !8

570:                                              ; preds = %550
  store i8 0, ptr %510, align 16
  store i8 0, ptr %511, align 1
  br i1 %87, label %571, label %594

571:                                              ; preds = %570, %571
  %572 = phi i64 [ %592, %571 ], [ 0, %570 ]
  %573 = getelementptr inbounds nuw i8, ptr %549, i64 %572
  %574 = load i8, ptr %573, align 1
  %575 = load i8, ptr %510, align 16
  %576 = add i8 %575, 1
  store i8 %576, ptr %510, align 16
  %577 = zext i8 %576 to i64
  %578 = getelementptr inbounds nuw i8, ptr %10, i64 %577
  %579 = load i8, ptr %578, align 1
  %580 = load i8, ptr %511, align 1
  %581 = add i8 %580, %579
  store i8 %581, ptr %511, align 1
  %582 = zext i8 %581 to i64
  %583 = getelementptr inbounds nuw i8, ptr %10, i64 %582
  %584 = load i8, ptr %583, align 1
  store i8 %584, ptr %578, align 1
  store i8 %579, ptr %583, align 1
  %585 = load i8, ptr %578, align 1
  %586 = add i8 %585, %579
  %587 = zext i8 %586 to i64
  %588 = getelementptr inbounds nuw i8, ptr %10, i64 %587
  %589 = load i8, ptr %588, align 1
  %590 = xor i8 %589, %574
  %591 = getelementptr inbounds nuw i8, ptr %548, i64 %572
  store i8 %590, ptr %591, align 1
  %592 = add nuw nsw i64 %572, 1
  %593 = icmp eq i64 %592, %512
  br i1 %593, label %594, label %571, !llvm.loop !10

594:                                              ; preds = %571, %570
  call void @llvm.lifetime.end.p0(ptr nonnull %10) #14
  %595 = add nuw nsw i32 %547, 1
  %596 = icmp eq i32 %595, 10
  br i1 %596, label %528, label %546, !llvm.loop !24

597:                                              ; preds = %648
  %598 = tail call i32 @memcmp(ptr noundef nonnull %21, ptr noundef %602, i64 noundef %80)
  %599 = icmp eq i32 %598, 0
  br i1 %599, label %654, label %651

600:                                              ; preds = %528, %648
  %601 = phi i32 [ 0, %528 ], [ %649, %648 ]
  %602 = phi ptr [ %549, %528 ], [ %603, %648 ]
  %603 = phi ptr [ %548, %528 ], [ %602, %648 ]
  call void @llvm.lifetime.start.p0(ptr nonnull %11) #14
  store <16 x i8> <i8 0, i8 1, i8 2, i8 3, i8 4, i8 5, i8 6, i8 7, i8 8, i8 9, i8 10, i8 11, i8 12, i8 13, i8 14, i8 15>, ptr %11, align 16
  store <16 x i8> <i8 16, i8 17, i8 18, i8 19, i8 20, i8 21, i8 22, i8 23, i8 24, i8 25, i8 26, i8 27, i8 28, i8 29, i8 30, i8 31>, ptr %531, align 16
  store <16 x i8> <i8 32, i8 33, i8 34, i8 35, i8 36, i8 37, i8 38, i8 39, i8 40, i8 41, i8 42, i8 43, i8 44, i8 45, i8 46, i8 47>, ptr %532, align 16
  store <16 x i8> <i8 48, i8 49, i8 50, i8 51, i8 52, i8 53, i8 54, i8 55, i8 56, i8 57, i8 58, i8 59, i8 60, i8 61, i8 62, i8 63>, ptr %533, align 16
  store <16 x i8> <i8 64, i8 65, i8 66, i8 67, i8 68, i8 69, i8 70, i8 71, i8 72, i8 73, i8 74, i8 75, i8 76, i8 77, i8 78, i8 79>, ptr %534, align 16
  store <16 x i8> <i8 80, i8 81, i8 82, i8 83, i8 84, i8 85, i8 86, i8 87, i8 88, i8 89, i8 90, i8 91, i8 92, i8 93, i8 94, i8 95>, ptr %535, align 16
  store <16 x i8> <i8 96, i8 97, i8 98, i8 99, i8 100, i8 101, i8 102, i8 103, i8 104, i8 105, i8 106, i8 107, i8 108, i8 109, i8 110, i8 111>, ptr %536, align 16
  store <16 x i8> <i8 112, i8 113, i8 114, i8 115, i8 116, i8 117, i8 118, i8 119, i8 120, i8 121, i8 122, i8 123, i8 124, i8 125, i8 126, i8 127>, ptr %537, align 16
  store <16 x i8> <i8 -128, i8 -127, i8 -126, i8 -125, i8 -124, i8 -123, i8 -122, i8 -121, i8 -120, i8 -119, i8 -118, i8 -117, i8 -116, i8 -115, i8 -114, i8 -113>, ptr %538, align 16
  store <16 x i8> <i8 -112, i8 -111, i8 -110, i8 -109, i8 -108, i8 -107, i8 -106, i8 -105, i8 -104, i8 -103, i8 -102, i8 -101, i8 -100, i8 -99, i8 -98, i8 -97>, ptr %539, align 16
  store <16 x i8> <i8 -96, i8 -95, i8 -94, i8 -93, i8 -92, i8 -91, i8 -90, i8 -89, i8 -88, i8 -87, i8 -86, i8 -85, i8 -84, i8 -83, i8 -82, i8 -81>, ptr %540, align 16
  store <16 x i8> <i8 -80, i8 -79, i8 -78, i8 -77, i8 -76, i8 -75, i8 -74, i8 -73, i8 -72, i8 -71, i8 -70, i8 -69, i8 -68, i8 -67, i8 -66, i8 -65>, ptr %541, align 16
  store <16 x i8> <i8 -64, i8 -63, i8 -62, i8 -61, i8 -60, i8 -59, i8 -58, i8 -57, i8 -56, i8 -55, i8 -54, i8 -53, i8 -52, i8 -51, i8 -50, i8 -49>, ptr %542, align 16
  store <16 x i8> <i8 -48, i8 -47, i8 -46, i8 -45, i8 -44, i8 -43, i8 -42, i8 -41, i8 -40, i8 -39, i8 -38, i8 -37, i8 -36, i8 -35, i8 -34, i8 -33>, ptr %543, align 16
  store <16 x i8> <i8 -32, i8 -31, i8 -30, i8 -29, i8 -28, i8 -27, i8 -26, i8 -25, i8 -24, i8 -23, i8 -22, i8 -21, i8 -20, i8 -19, i8 -18, i8 -17>, ptr %544, align 16
  store <16 x i8> <i8 -16, i8 -15, i8 -14, i8 -13, i8 -12, i8 -11, i8 -10, i8 -9, i8 -8, i8 -7, i8 -6, i8 -5, i8 -4, i8 -3, i8 -2, i8 -1>, ptr %545, align 16
  br label %604

604:                                              ; preds = %600, %604
  %605 = phi i64 [ %622, %604 ], [ 0, %600 ]
  %606 = phi i32 [ %617, %604 ], [ 0, %600 ]
  %607 = getelementptr inbounds nuw i8, ptr %11, i64 %605
  %608 = load i8, ptr %607, align 1
  %609 = zext i8 %608 to i32
  %610 = add i32 %606, %609
  %611 = trunc nuw nsw i64 %605 to i32
  %612 = srem i32 %611, %23
  %613 = zext nneg i32 %612 to i64
  %614 = getelementptr inbounds nuw i8, ptr %19, i64 %613
  %615 = load i8, ptr %614, align 1
  %616 = zext i8 %615 to i32
  %617 = add i32 %610, %616
  %618 = and i32 %617, 255
  %619 = zext nneg i32 %618 to i64
  %620 = getelementptr inbounds nuw i8, ptr %11, i64 %619
  %621 = load i8, ptr %620, align 1
  store i8 %621, ptr %607, align 1
  store i8 %608, ptr %620, align 1
  %622 = add nuw nsw i64 %605, 1
  %623 = icmp eq i64 %622, 256
  br i1 %623, label %624, label %604, !llvm.loop !8

624:                                              ; preds = %604
  store i8 0, ptr %529, align 16
  store i8 0, ptr %530, align 1
  br i1 %87, label %625, label %648

625:                                              ; preds = %624, %625
  %626 = phi i64 [ %646, %625 ], [ 0, %624 ]
  %627 = getelementptr inbounds nuw i8, ptr %603, i64 %626
  %628 = load i8, ptr %627, align 1
  %629 = load i8, ptr %529, align 16
  %630 = add i8 %629, 1
  store i8 %630, ptr %529, align 16
  %631 = zext i8 %630 to i64
  %632 = getelementptr inbounds nuw i8, ptr %11, i64 %631
  %633 = load i8, ptr %632, align 1
  %634 = load i8, ptr %530, align 1
  %635 = add i8 %634, %633
  store i8 %635, ptr %530, align 1
  %636 = zext i8 %635 to i64
  %637 = getelementptr inbounds nuw i8, ptr %11, i64 %636
  %638 = load i8, ptr %637, align 1
  store i8 %638, ptr %632, align 1
  store i8 %633, ptr %637, align 1
  %639 = load i8, ptr %632, align 1
  %640 = add i8 %639, %633
  %641 = zext i8 %640 to i64
  %642 = getelementptr inbounds nuw i8, ptr %11, i64 %641
  %643 = load i8, ptr %642, align 1
  %644 = xor i8 %643, %628
  %645 = getelementptr inbounds nuw i8, ptr %602, i64 %626
  store i8 %644, ptr %645, align 1
  %646 = add nuw nsw i64 %626, 1
  %647 = icmp eq i64 %646, %512
  br i1 %647, label %648, label %625, !llvm.loop !10

648:                                              ; preds = %625, %624
  call void @llvm.lifetime.end.p0(ptr nonnull %11) #14
  %649 = add nuw nsw i32 %601, 1
  %650 = icmp eq i32 %649, 10
  br i1 %650, label %597, label %600, !llvm.loop !25

651:                                              ; preds = %597
  %652 = tail call ptr @__acrt_iob_func(i32 noundef 2) #14
  %653 = tail call i64 @fwrite(ptr nonnull @"??_C@_0CK@EPPHFHAG@FAIL?3?5Multi?9round?5encrypt?1decryp@", i64 41, i64 1, ptr %652)
  br label %656

654:                                              ; preds = %597
  %655 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  br label %656

656:                                              ; preds = %651, %654, %502
  %657 = phi i32 [ 1, %651 ], [ %401, %654 ], [ %401, %502 ]
  %658 = phi ptr [ %602, %651 ], [ %602, %654 ], [ %504, %502 ]
  %659 = phi ptr [ %603, %651 ], [ %603, %654 ], [ %505, %502 ]
  tail call void @free(ptr noundef %658)
  tail call void @free(ptr noundef %659)
  %660 = icmp eq i32 %657, 0
  %661 = select i1 %660, ptr @str.3, ptr @str.4
  %662 = tail call i32 @puts(ptr nonnull dereferenceable(1) %661)
  tail call void @free(ptr noundef %81)
  tail call void @free(ptr noundef %221)
  call void @llvm.lifetime.end.p0(ptr nonnull %9) #14
  call void @llvm.lifetime.end.p0(ptr nonnull %8) #14
  br label %663

663:                                              ; preds = %656, %223
  %664 = phi i32 [ %657, %656 ], [ 1, %223 ]
  call void @llvm.lifetime.end.p0(ptr nonnull %7) #14
  br label %665

665:                                              ; preds = %663, %83
  %666 = phi i32 [ %664, %663 ], [ 1, %83 ]
  call void @llvm.lifetime.end.p0(ptr nonnull %6)
  call void @llvm.lifetime.end.p0(ptr nonnull %5) #14
  br label %667

667:                                              ; preds = %29, %34, %665, %13
  %668 = phi i32 [ 1, %13 ], [ 1, %29 ], [ 1, %34 ], [ %666, %665 ]
  ret i32 %668
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @fprintf(ptr noundef %0, ptr noundef %1, ...) local_unnamed_addr #1 comdat {
  %3 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %3) #14
  call void @llvm.va_start.p0(ptr nonnull %3)
  %4 = load ptr, ptr %3, align 8
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %0, ptr noundef %1, ptr noundef null, ptr noundef %4) #14
  call void @llvm.va_end.p0(ptr nonnull %3)
  call void @llvm.lifetime.end.p0(ptr nonnull %3) #14
  ret i32 %7
}

declare dso_local ptr @__acrt_iob_func(i32 noundef) local_unnamed_addr #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #3

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: read)
declare dso_local i64 @strlen(ptr noundef captures(none)) local_unnamed_addr #4

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @printf(ptr noundef %0, ...) local_unnamed_addr #1 comdat {
  %2 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %2) #14
  call void @llvm.va_start.p0(ptr nonnull %2)
  %3 = load ptr, ptr %2, align 8
  %4 = call ptr @__acrt_iob_func(i32 noundef 1) #14
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %4, ptr noundef %0, ptr noundef null, ptr noundef %3) #14
  call void @llvm.va_end.p0(ptr nonnull %2)
  call void @llvm.lifetime.end.p0(ptr nonnull %2) #14
  ret i32 %7
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #5

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite)
declare dso_local noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #6

; Function Attrs: mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite)
declare dso_local void @free(ptr allocptr noundef captures(none)) local_unnamed_addr #7

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: read)
declare dso_local i32 @memcmp(ptr noundef captures(none), ptr noundef captures(none), i64 noundef) local_unnamed_addr #4

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

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #10

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #11

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #11

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #11

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smin.i32(i32, i32) #12

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.fshl.i32(i32, i32, i32) #12

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i8 @llvm.fshl.i8(i8, i8, i8) #12

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare <16 x i8> @llvm.fshl.v16i8(<16 x i8>, <16 x i8>, <16 x i8>) #12

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare <4 x i8> @llvm.fshl.v4i8(<4 x i8>, <4 x i8>, <4 x i8>) #12

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef) #13

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { inlinehint nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: read) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #6 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { mustprogress nocallback nofree nosync nounwind willreturn }
attributes #9 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #10 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #11 = { nofree nounwind }
attributes #12 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #13 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #14 = { nounwind }
attributes #15 = { allocsize(0) }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 22.1.2 (https://github.com/llvm/llvm-project 1ab49a973e210e97d61e5db6557180dcb92c3e98)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "rc4.c", directory: "C:\\Users\\Treverm\\Documents\\Claude Code\\llvm-obfuscator\\eval\\benchmarks")
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
!13 = distinct !{!13, !9, !14, !15}
!14 = !{!"llvm.loop.isvectorized", i32 1}
!15 = !{!"llvm.loop.unroll.runtime.disable"}
!16 = !{!"branch_weights", i32 4, i32 12}
!17 = distinct !{!17, !9, !14, !15}
!18 = distinct !{!18, !9, !15, !14}
!19 = distinct !{!19, !9, !14, !15}
!20 = distinct !{!20, !9, !14, !15}
!21 = distinct !{!21, !9, !15, !14}
!22 = distinct !{!22, !9}
!23 = distinct !{!23, !9}
!24 = distinct !{!24, !9}
!25 = distinct !{!25, !9}
