; ModuleID = 'hashtable.c'
source_filename = "hashtable.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.44.35224"

%struct.HTEntry = type { ptr, i32, i32 }

$fprintf = comdat any

$printf = comdat any

$__local_stdio_printf_options = comdat any

$"??_C@_0BN@MJIOAPKE@Failed?5to?5create?5hash?5table?6?$AA@" = comdat any

$"??_C@_0CB@BAHDAGOF@Words?5inserted?3?5?$CFd?5?$CIunique?3?5?$CFd?$CJ?6@" = comdat any

$"??_C@_0BO@MGMADAMK@Table?5size?3?5?$CFd?0?5capacity?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_0BG@PLKHCFJJ@Input?5checksum?3?5?$CF08x?6?$AA@" = comdat any

$"??_C@_05IAEKHIAN@alpha?$AA@" = comdat any

$"??_C@_07BCCGMNOK@charlie?$AA@" = comdat any

$"??_C@_04DOALKIP@golf?$AA@" = comdat any

$"??_C@_06CGCFEEDE@banana?$AA@" = comdat any

$"??_C@_0CH@EIIFJFPB@FAIL?3?5?8?$CFs?8?5still?5found?5after?5del@" = comdat any

$"??_C@_0CJ@HDNIDCA@FAIL?3?5?8?$CFs?8?5not?5found?5after?5re?9in@" = comdat any

$"??_C@_0BG@ELBGGNKF@Hits?3?5?$CFd?0?5Misses?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_03ICICOMAL@yes?$AA@" = comdat any

$"??_C@_02KAJCLHKP@no?$AA@" = comdat any

$"??_C@_0CC@NIKOCKLC@Deleted?3?5?$CFd?0?5Delete?5verified?3?5?$CFs@" = comdat any

$"??_C@_0BI@EIGPOAFK@Re?9insert?5verified?3?5?$CFs?6?$AA@" = comdat any

$"??_C@_0BH@JIKJOHDO@Lookup?5checksum?3?5?$CF08x?6?$AA@" = comdat any

$"??_C@_0BC@JJCOHFPB@Total?5probes?3?5?$CFu?6?$AA@" = comdat any

$"??_C@_0DK@IJMNHKLC@FAIL?3?5size?5mismatch?5after?5operat@" = comdat any

$"??_C@_05LJJALAEO@bravo?$AA@" = comdat any

$"??_C@_05NCGEDJPM@delta?$AA@" = comdat any

$"??_C@_04OFDNEFMC@echo?$AA@" = comdat any

$"??_C@_07NDGKCEPN@foxtrot?$AA@" = comdat any

$"??_C@_05NCPBCJFM@hotel?$AA@" = comdat any

$"??_C@_05MIGAADPP@india?$AA@" = comdat any

$"??_C@_06JBJDIKE@juliet?$AA@" = comdat any

$"??_C@_04DCLJLDBL@kilo?$AA@" = comdat any

$"??_C@_04BPNICILC@lima?$AA@" = comdat any

$"??_C@_04ECFJLILE@mike?$AA@" = comdat any

$"??_C@_08PAIFOGED@november?$AA@" = comdat any

$"??_C@_05HEGCJLEC@oscar?$AA@" = comdat any

$"??_C@_04GLICEKPN@papa?$AA@" = comdat any

$"??_C@_06LDFCJBHG@quebec?$AA@" = comdat any

$"??_C@_05HGIAEDHH@romeo?$AA@" = comdat any

$"??_C@_06CLJPMCFH@sierra?$AA@" = comdat any

$"??_C@_05ONIGFIL@tango?$AA@" = comdat any

$"??_C@_07HIAOBKCE@uniform?$AA@" = comdat any

$"??_C@_06KGJNDGDN@victor?$AA@" = comdat any

$"??_C@_07EJFOMFGG@whiskey?$AA@" = comdat any

$"??_C@_04IGKBKIFD@xray?$AA@" = comdat any

$"??_C@_06EEHIEGIB@yankee?$AA@" = comdat any

$"??_C@_04MFNLBPNF@zulu?$AA@" = comdat any

$"??_C@_05EGDPGPFG@apple?$AA@" = comdat any

$"??_C@_06FAGBFGF@cherry?$AA@" = comdat any

$"??_C@_04JNIHBGGH@date?$AA@" = comdat any

$"??_C@_0L@OAADGPBI@elderberry?$AA@" = comdat any

$"??_C@_03KNEMIFPD@fig?$AA@" = comdat any

$"??_C@_05BHDPLFMF@grape?$AA@" = comdat any

$"??_C@_08BKNCNHLL@honeydew?$AA@" = comdat any

$"??_C@_04HEJAFIAM@kiwi?$AA@" = comdat any

$"??_C@_05BBGAIMKK@lemon?$AA@" = comdat any

$"??_C@_05CKABDGNI@mango?$AA@" = comdat any

$"??_C@_09NPLLJKNA@nectarine?$AA@" = comdat any

$"??_C@_06HDDOCMMN@orange?$AA@" = comdat any

$"??_C@_06BANNFFMF@papaya?$AA@" = comdat any

$"??_C@_06GOKDBIPL@quince?$AA@" = comdat any

$"??_C@_09BHDGFBGJ@raspberry?$AA@" = comdat any

$"??_C@_0L@JLJBLKCK@strawberry?$AA@" = comdat any

$"??_C@_09OEHJALGJ@tangerine?$AA@" = comdat any

$"??_C@_0L@BPEKPNBP@watermelon?$AA@" = comdat any

$"??_C@_09JPNFBOJG@algorithm?$AA@" = comdat any

$"??_C@_09HHEPMDCB@benchmark?$AA@" = comdat any

$"??_C@_08CCFHILPL@compiler?$AA@" = comdat any

$"??_C@_08BJECCCMA@debugger?$AA@" = comdat any

$"??_C@_07KLAMFGHC@encoder?$AA@" = comdat any

$"??_C@_09EJALMONM@framework?$AA@" = comdat any

$"??_C@_09HNDCMPDF@generator?$AA@" = comdat any

$"??_C@_07DPIAGIBF@hashmap?$AA@" = comdat any

$"??_C@_08EAMFHCBD@iterator?$AA@" = comdat any

$"??_C@_08JDGLABGB@junction?$AA@" = comdat any

$"??_C@_05POKFBJG@zebra?$AA@" = comdat any

$"??_C@_07NCACNKHJ@missing?$AA@" = comdat any

$"??_C@_07FILEPJKF@nothere?$AA@" = comdat any

$"??_C@_07BMPLAIGL@phantom?$AA@" = comdat any

$"??_C@_07CIFAGBMG@unknown?$AA@" = comdat any

$"??_C@_03GBBIHDEJ@foo?$AA@" = comdat any

@"??_C@_0BN@MJIOAPKE@Failed?5to?5create?5hash?5table?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [29 x i8] c"Failed to create hash table\0A\00", comdat, align 1
@builtin_words = internal unnamed_addr constant [56 x ptr] [ptr @"??_C@_05IAEKHIAN@alpha?$AA@", ptr @"??_C@_05LJJALAEO@bravo?$AA@", ptr @"??_C@_07BCCGMNOK@charlie?$AA@", ptr @"??_C@_05NCGEDJPM@delta?$AA@", ptr @"??_C@_04OFDNEFMC@echo?$AA@", ptr @"??_C@_07NDGKCEPN@foxtrot?$AA@", ptr @"??_C@_04DOALKIP@golf?$AA@", ptr @"??_C@_05NCPBCJFM@hotel?$AA@", ptr @"??_C@_05MIGAADPP@india?$AA@", ptr @"??_C@_06JBJDIKE@juliet?$AA@", ptr @"??_C@_04DCLJLDBL@kilo?$AA@", ptr @"??_C@_04BPNICILC@lima?$AA@", ptr @"??_C@_04ECFJLILE@mike?$AA@", ptr @"??_C@_08PAIFOGED@november?$AA@", ptr @"??_C@_05HEGCJLEC@oscar?$AA@", ptr @"??_C@_04GLICEKPN@papa?$AA@", ptr @"??_C@_06LDFCJBHG@quebec?$AA@", ptr @"??_C@_05HGIAEDHH@romeo?$AA@", ptr @"??_C@_06CLJPMCFH@sierra?$AA@", ptr @"??_C@_05ONIGFIL@tango?$AA@", ptr @"??_C@_07HIAOBKCE@uniform?$AA@", ptr @"??_C@_06KGJNDGDN@victor?$AA@", ptr @"??_C@_07EJFOMFGG@whiskey?$AA@", ptr @"??_C@_04IGKBKIFD@xray?$AA@", ptr @"??_C@_06EEHIEGIB@yankee?$AA@", ptr @"??_C@_04MFNLBPNF@zulu?$AA@", ptr @"??_C@_05EGDPGPFG@apple?$AA@", ptr @"??_C@_06CGCFEEDE@banana?$AA@", ptr @"??_C@_06FAGBFGF@cherry?$AA@", ptr @"??_C@_04JNIHBGGH@date?$AA@", ptr @"??_C@_0L@OAADGPBI@elderberry?$AA@", ptr @"??_C@_03KNEMIFPD@fig?$AA@", ptr @"??_C@_05BHDPLFMF@grape?$AA@", ptr @"??_C@_08BKNCNHLL@honeydew?$AA@", ptr @"??_C@_04HEJAFIAM@kiwi?$AA@", ptr @"??_C@_05BBGAIMKK@lemon?$AA@", ptr @"??_C@_05CKABDGNI@mango?$AA@", ptr @"??_C@_09NPLLJKNA@nectarine?$AA@", ptr @"??_C@_06HDDOCMMN@orange?$AA@", ptr @"??_C@_06BANNFFMF@papaya?$AA@", ptr @"??_C@_06GOKDBIPL@quince?$AA@", ptr @"??_C@_09BHDGFBGJ@raspberry?$AA@", ptr @"??_C@_0L@JLJBLKCK@strawberry?$AA@", ptr @"??_C@_09OEHJALGJ@tangerine?$AA@", ptr @"??_C@_0L@BPEKPNBP@watermelon?$AA@", ptr @"??_C@_09JPNFBOJG@algorithm?$AA@", ptr @"??_C@_09HHEPMDCB@benchmark?$AA@", ptr @"??_C@_08CCFHILPL@compiler?$AA@", ptr @"??_C@_08BJECCCMA@debugger?$AA@", ptr @"??_C@_07KLAMFGHC@encoder?$AA@", ptr @"??_C@_09EJALMONM@framework?$AA@", ptr @"??_C@_09HNDCMPDF@generator?$AA@", ptr @"??_C@_07DPIAGIBF@hashmap?$AA@", ptr @"??_C@_08EAMFHCBD@iterator?$AA@", ptr @"??_C@_08JDGLABGB@junction?$AA@", ptr null], align 16
@"??_C@_0CB@BAHDAGOF@Words?5inserted?3?5?$CFd?5?$CIunique?3?5?$CFd?$CJ?6@" = linkonce_odr dso_local unnamed_addr constant [33 x i8] c"Words inserted: %d (unique: %d)\0A\00", comdat, align 1
@"??_C@_0BO@MGMADAMK@Table?5size?3?5?$CFd?0?5capacity?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [30 x i8] c"Table size: %d, capacity: %d\0A\00", comdat, align 1
@"??_C@_0BG@PLKHCFJJ@Input?5checksum?3?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [22 x i8] c"Input checksum: %08x\0A\00", comdat, align 1
@test_lookups = internal unnamed_addr constant [16 x ptr] [ptr @"??_C@_05IAEKHIAN@alpha?$AA@", ptr @"??_C@_05POKFBJG@zebra?$AA@", ptr @"??_C@_07BCCGMNOK@charlie?$AA@", ptr @"??_C@_07NCACNKHJ@missing?$AA@", ptr @"??_C@_04DOALKIP@golf?$AA@", ptr @"??_C@_07FILEPJKF@nothere?$AA@", ptr @"??_C@_04GLICEKPN@papa?$AA@", ptr @"??_C@_04IGKBKIFD@xray?$AA@", ptr @"??_C@_08CCFHILPL@compiler?$AA@", ptr @"??_C@_07BMPLAIGL@phantom?$AA@", ptr @"??_C@_06CGCFEEDE@banana?$AA@", ptr @"??_C@_07CIFAGBMG@unknown?$AA@", ptr @"??_C@_0L@BPEKPNBP@watermelon?$AA@", ptr @"??_C@_03GBBIHDEJ@foo?$AA@", ptr @"??_C@_05ONIGFIL@tango?$AA@", ptr null], align 16
@"??_C@_05IAEKHIAN@alpha?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"alpha\00", comdat, align 1
@"??_C@_07BCCGMNOK@charlie?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"charlie\00", comdat, align 1
@"??_C@_04DOALKIP@golf?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"golf\00", comdat, align 1
@"??_C@_06CGCFEEDE@banana?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"banana\00", comdat, align 1
@__const.main.to_delete = private unnamed_addr constant [5 x ptr] [ptr @"??_C@_05IAEKHIAN@alpha?$AA@", ptr @"??_C@_07BCCGMNOK@charlie?$AA@", ptr @"??_C@_04DOALKIP@golf?$AA@", ptr @"??_C@_06CGCFEEDE@banana?$AA@", ptr null], align 16
@"??_C@_0CH@EIIFJFPB@FAIL?3?5?8?$CFs?8?5still?5found?5after?5del@" = linkonce_odr dso_local unnamed_addr constant [39 x i8] c"FAIL: '%s' still found after deletion\0A\00", comdat, align 1
@"??_C@_0CJ@HDNIDCA@FAIL?3?5?8?$CFs?8?5not?5found?5after?5re?9in@" = linkonce_odr dso_local unnamed_addr constant [41 x i8] c"FAIL: '%s' not found after re-insertion\0A\00", comdat, align 1
@"??_C@_0BG@ELBGGNKF@Hits?3?5?$CFd?0?5Misses?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [22 x i8] c"Hits: %d, Misses: %d\0A\00", comdat, align 1
@"??_C@_03ICICOMAL@yes?$AA@" = linkonce_odr dso_local unnamed_addr constant [4 x i8] c"yes\00", comdat, align 1
@"??_C@_02KAJCLHKP@no?$AA@" = linkonce_odr dso_local unnamed_addr constant [3 x i8] c"no\00", comdat, align 1
@"??_C@_0CC@NIKOCKLC@Deleted?3?5?$CFd?0?5Delete?5verified?3?5?$CFs@" = linkonce_odr dso_local unnamed_addr constant [34 x i8] c"Deleted: %d, Delete verified: %s\0A\00", comdat, align 1
@"??_C@_0BI@EIGPOAFK@Re?9insert?5verified?3?5?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [24 x i8] c"Re-insert verified: %s\0A\00", comdat, align 1
@"??_C@_0BH@JIKJOHDO@Lookup?5checksum?3?5?$CF08x?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [23 x i8] c"Lookup checksum: %08x\0A\00", comdat, align 1
@"??_C@_0BC@JJCOHFPB@Total?5probes?3?5?$CFu?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [18 x i8] c"Total probes: %u\0A\00", comdat, align 1
@"??_C@_0DK@IJMNHKLC@FAIL?3?5size?5mismatch?5after?5operat@" = linkonce_odr dso_local unnamed_addr constant [58 x i8] c"FAIL: size mismatch after operations: got %d expected %d\0A\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@"??_C@_05LJJALAEO@bravo?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"bravo\00", comdat, align 1
@"??_C@_05NCGEDJPM@delta?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"delta\00", comdat, align 1
@"??_C@_04OFDNEFMC@echo?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"echo\00", comdat, align 1
@"??_C@_07NDGKCEPN@foxtrot?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"foxtrot\00", comdat, align 1
@"??_C@_05NCPBCJFM@hotel?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"hotel\00", comdat, align 1
@"??_C@_05MIGAADPP@india?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"india\00", comdat, align 1
@"??_C@_06JBJDIKE@juliet?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"juliet\00", comdat, align 1
@"??_C@_04DCLJLDBL@kilo?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"kilo\00", comdat, align 1
@"??_C@_04BPNICILC@lima?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"lima\00", comdat, align 1
@"??_C@_04ECFJLILE@mike?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"mike\00", comdat, align 1
@"??_C@_08PAIFOGED@november?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"november\00", comdat, align 1
@"??_C@_05HEGCJLEC@oscar?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"oscar\00", comdat, align 1
@"??_C@_04GLICEKPN@papa?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"papa\00", comdat, align 1
@"??_C@_06LDFCJBHG@quebec?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"quebec\00", comdat, align 1
@"??_C@_05HGIAEDHH@romeo?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"romeo\00", comdat, align 1
@"??_C@_06CLJPMCFH@sierra?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"sierra\00", comdat, align 1
@"??_C@_05ONIGFIL@tango?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"tango\00", comdat, align 1
@"??_C@_07HIAOBKCE@uniform?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"uniform\00", comdat, align 1
@"??_C@_06KGJNDGDN@victor?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"victor\00", comdat, align 1
@"??_C@_07EJFOMFGG@whiskey?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"whiskey\00", comdat, align 1
@"??_C@_04IGKBKIFD@xray?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"xray\00", comdat, align 1
@"??_C@_06EEHIEGIB@yankee?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"yankee\00", comdat, align 1
@"??_C@_04MFNLBPNF@zulu?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"zulu\00", comdat, align 1
@"??_C@_05EGDPGPFG@apple?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"apple\00", comdat, align 1
@"??_C@_06FAGBFGF@cherry?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"cherry\00", comdat, align 1
@"??_C@_04JNIHBGGH@date?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"date\00", comdat, align 1
@"??_C@_0L@OAADGPBI@elderberry?$AA@" = linkonce_odr dso_local unnamed_addr constant [11 x i8] c"elderberry\00", comdat, align 1
@"??_C@_03KNEMIFPD@fig?$AA@" = linkonce_odr dso_local unnamed_addr constant [4 x i8] c"fig\00", comdat, align 1
@"??_C@_05BHDPLFMF@grape?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"grape\00", comdat, align 1
@"??_C@_08BKNCNHLL@honeydew?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"honeydew\00", comdat, align 1
@"??_C@_04HEJAFIAM@kiwi?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"kiwi\00", comdat, align 1
@"??_C@_05BBGAIMKK@lemon?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"lemon\00", comdat, align 1
@"??_C@_05CKABDGNI@mango?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"mango\00", comdat, align 1
@"??_C@_09NPLLJKNA@nectarine?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"nectarine\00", comdat, align 1
@"??_C@_06HDDOCMMN@orange?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"orange\00", comdat, align 1
@"??_C@_06BANNFFMF@papaya?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"papaya\00", comdat, align 1
@"??_C@_06GOKDBIPL@quince?$AA@" = linkonce_odr dso_local unnamed_addr constant [7 x i8] c"quince\00", comdat, align 1
@"??_C@_09BHDGFBGJ@raspberry?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"raspberry\00", comdat, align 1
@"??_C@_0L@JLJBLKCK@strawberry?$AA@" = linkonce_odr dso_local unnamed_addr constant [11 x i8] c"strawberry\00", comdat, align 1
@"??_C@_09OEHJALGJ@tangerine?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"tangerine\00", comdat, align 1
@"??_C@_0L@BPEKPNBP@watermelon?$AA@" = linkonce_odr dso_local unnamed_addr constant [11 x i8] c"watermelon\00", comdat, align 1
@"??_C@_09JPNFBOJG@algorithm?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"algorithm\00", comdat, align 1
@"??_C@_09HHEPMDCB@benchmark?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"benchmark\00", comdat, align 1
@"??_C@_08CCFHILPL@compiler?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"compiler\00", comdat, align 1
@"??_C@_08BJECCCMA@debugger?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"debugger\00", comdat, align 1
@"??_C@_07KLAMFGHC@encoder?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"encoder\00", comdat, align 1
@"??_C@_09EJALMONM@framework?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"framework\00", comdat, align 1
@"??_C@_09HNDCMPDF@generator?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"generator\00", comdat, align 1
@"??_C@_07DPIAGIBF@hashmap?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"hashmap\00", comdat, align 1
@"??_C@_08EAMFHCBD@iterator?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"iterator\00", comdat, align 1
@"??_C@_08JDGLABGB@junction?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"junction\00", comdat, align 1
@"??_C@_05POKFBJG@zebra?$AA@" = linkonce_odr dso_local unnamed_addr constant [6 x i8] c"zebra\00", comdat, align 1
@"??_C@_07NCACNKHJ@missing?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"missing\00", comdat, align 1
@"??_C@_07FILEPJKF@nothere?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"nothere\00", comdat, align 1
@"??_C@_07BMPLAIGL@phantom?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"phantom\00", comdat, align 1
@"??_C@_07CIFAGBMG@unknown?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"unknown\00", comdat, align 1
@"??_C@_03GBBIHDEJ@foo?$AA@" = linkonce_odr dso_local unnamed_addr constant [4 x i8] c"foo\00", comdat, align 1
@str = private unnamed_addr constant [29 x i8] c"=== Hash Table Benchmark ===\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"RESULT: PASS\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"RESULT: FAIL\00", align 1

; Function Attrs: nounwind uwtable
define dso_local range(i32 0, 2) i32 @main(i32 noundef %0, ptr noundef readnone captures(none) %1) local_unnamed_addr #0 {
  %3 = alloca [256 x i8], align 16
  %4 = alloca [256 x i8], align 16
  %5 = alloca [256 x i8], align 16
  %6 = alloca [4 x i32], align 16
  %7 = tail call noalias dereferenceable_or_null(32) ptr @calloc(i64 noundef 1, i64 noundef 32) #17
  %8 = icmp eq ptr %7, null
  br i1 %8, label %13, label %9

9:                                                ; preds = %2
  %10 = tail call noalias dereferenceable_or_null(1024) ptr @calloc(i64 noundef 64, i64 noundef 16) #17
  store ptr %10, ptr %7, align 8
  %11 = icmp eq ptr %10, null
  br i1 %11, label %12, label %16

12:                                               ; preds = %9
  tail call void @free(ptr noundef nonnull %7)
  br label %13

13:                                               ; preds = %12, %2
  %14 = tail call ptr @__acrt_iob_func(i32 noundef 2) #18
  %15 = tail call i64 @fwrite(ptr nonnull @"??_C@_0BN@MJIOAPKE@Failed?5to?5create?5hash?5table?6?$AA@", i64 28, i64 1, ptr %14)
  br label %630

16:                                               ; preds = %9
  %17 = getelementptr inbounds nuw i8, ptr %7, i64 8
  store i32 64, ptr %17, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %3) #18
  %18 = tail call i32 @_isatty(i32 noundef 0) #18
  %19 = icmp eq i32 %18, 0
  br i1 %19, label %20, label %90

20:                                               ; preds = %16, %86
  %21 = phi i1 [ false, %86 ], [ true, %16 ]
  %22 = phi i32 [ %87, %86 ], [ 0, %16 ]
  %23 = phi i32 [ %88, %86 ], [ 0, %16 ]
  %24 = icmp samesign ult i32 %23, 100000
  br label %25

25:                                               ; preds = %20, %45
  %26 = call ptr @__acrt_iob_func(i32 noundef 0) #18
  %27 = call ptr @fgets(ptr noundef nonnull %3, i32 noundef 256, ptr noundef %26)
  %28 = icmp ne ptr %27, null
  %29 = and i1 %24, %28
  br i1 %29, label %30, label %89

30:                                               ; preds = %25
  %31 = call i64 @strlen(ptr noundef nonnull dereferenceable(1) %3) #18
  %32 = trunc i64 %31 to i32
  %33 = icmp sgt i32 %32, 0
  br i1 %33, label %34, label %45

34:                                               ; preds = %30
  %35 = and i64 %31, 2147483647
  br label %36

36:                                               ; preds = %41, %34
  %37 = phi i64 [ %35, %34 ], [ %42, %41 ]
  %38 = getelementptr i8, ptr %3, i64 %37
  %39 = getelementptr i8, ptr %38, i64 -1
  %40 = load i8, ptr %39, align 1
  switch i8 %40, label %45 [
    i8 10, label %41
    i8 13, label %41
    i8 32, label %41
    i8 9, label %41
  ]

41:                                               ; preds = %36, %36, %36, %36
  %42 = add nsw i64 %37, -1
  %43 = getelementptr inbounds nuw i8, ptr %3, i64 %42
  store i8 0, ptr %43, align 1
  %44 = icmp sgt i64 %37, 1
  br i1 %44, label %36, label %45, !llvm.loop !8

45:                                               ; preds = %36, %41, %30
  %46 = load i8, ptr %3, align 16
  %47 = icmp eq i8 %46, 0
  br i1 %47, label %25, label %48, !llvm.loop !10

48:                                               ; preds = %45
  call void @llvm.lifetime.start.p0(ptr nonnull %4) #18
  br label %49

49:                                               ; preds = %48, %49
  %50 = phi i64 [ %57, %49 ], [ 0, %48 ]
  %51 = phi i8 [ %59, %49 ], [ %46, %48 ]
  %52 = add i8 %51, -65
  %53 = icmp ult i8 %52, 26
  %54 = or disjoint i8 %51, 32
  %55 = select i1 %53, i8 %54, i8 %51
  %56 = getelementptr inbounds nuw i8, ptr %4, i64 %50
  store i8 %55, ptr %56, align 1
  %57 = add nuw nsw i64 %50, 1
  %58 = getelementptr inbounds nuw i8, ptr %3, i64 %57
  %59 = load i8, ptr %58, align 1
  %60 = icmp ne i8 %59, 0
  %61 = icmp samesign ult i64 %50, 254
  %62 = select i1 %60, i1 %61, i1 false
  br i1 %62, label %49, label %63, !llvm.loop !11

63:                                               ; preds = %49
  %64 = getelementptr inbounds nuw i8, ptr %4, i64 %57
  store i8 0, ptr %64, align 1
  %65 = load i8, ptr %4, align 16
  %66 = icmp eq i8 %65, 0
  br i1 %66, label %86, label %67

67:                                               ; preds = %63, %67
  %68 = phi i8 [ %82, %67 ], [ %65, %63 ]
  %69 = phi ptr [ %72, %67 ], [ %4, %63 ]
  %70 = phi i32 [ %81, %67 ], [ 0, %63 ]
  %71 = phi i32 [ %80, %67 ], [ 0, %63 ]
  %72 = getelementptr inbounds nuw i8, ptr %69, i64 1
  %73 = zext i8 %68 to i32
  %74 = and i32 %70, 255
  %75 = xor i32 %74, %73
  %76 = shl nuw nsw i32 %75, 5
  %77 = xor i32 %76, %71
  %78 = call i32 @llvm.fshl.i32(i32 %77, i32 %77, i32 3)
  %79 = mul i32 %75, -957401568
  %80 = add i32 %78, %79
  %81 = add nuw nsw i32 %70, 1
  %82 = load i8, ptr %72, align 1
  %83 = icmp eq i8 %82, 0
  br i1 %83, label %84, label %67, !llvm.loop !12

84:                                               ; preds = %67
  %85 = xor i32 %80, %22
  br label %86

86:                                               ; preds = %84, %63
  %87 = phi i32 [ %22, %63 ], [ %85, %84 ]
  %88 = add nuw nsw i32 %23, 1
  call fastcc void @ht_insert(ptr noundef %7, ptr noundef nonnull %4, i32 noundef %88)
  call void @llvm.lifetime.end.p0(ptr nonnull %4) #18
  br label %20, !llvm.loop !10

89:                                               ; preds = %25
  br i1 %21, label %90, label %126

90:                                               ; preds = %16, %89
  %91 = phi i32 [ %23, %89 ], [ 0, %16 ]
  %92 = phi i32 [ %22, %89 ], [ 0, %16 ]
  br label %93

93:                                               ; preds = %90, %119
  %94 = phi i64 [ 0, %90 ], [ %121, %119 ]
  %95 = phi i32 [ %92, %90 ], [ %120, %119 ]
  %96 = getelementptr inbounds nuw ptr, ptr @builtin_words, i64 %94
  %97 = load ptr, ptr %96, align 8
  %98 = load i8, ptr %97, align 1
  %99 = icmp eq i8 %98, 0
  br i1 %99, label %119, label %100

100:                                              ; preds = %93, %100
  %101 = phi i8 [ %115, %100 ], [ %98, %93 ]
  %102 = phi ptr [ %105, %100 ], [ %97, %93 ]
  %103 = phi i32 [ %114, %100 ], [ 0, %93 ]
  %104 = phi i32 [ %113, %100 ], [ 0, %93 ]
  %105 = getelementptr inbounds nuw i8, ptr %102, i64 1
  %106 = zext i8 %101 to i32
  %107 = and i32 %103, 255
  %108 = xor i32 %107, %106
  %109 = shl nuw nsw i32 %108, 5
  %110 = xor i32 %109, %104
  %111 = call i32 @llvm.fshl.i32(i32 %110, i32 %110, i32 3)
  %112 = mul i32 %108, -957401568
  %113 = add i32 %111, %112
  %114 = add nuw nsw i32 %103, 1
  %115 = load i8, ptr %105, align 1
  %116 = icmp eq i8 %115, 0
  br i1 %116, label %117, label %100, !llvm.loop !12

117:                                              ; preds = %100
  %118 = xor i32 %113, %95
  br label %119

119:                                              ; preds = %117, %93
  %120 = phi i32 [ %95, %93 ], [ %118, %117 ]
  %121 = add nuw nsw i64 %94, 1
  %122 = trunc nuw nsw i64 %121 to i32
  call fastcc void @ht_insert(ptr noundef %7, ptr noundef nonnull %97, i32 noundef %122)
  %123 = icmp eq i64 %121, 55
  br i1 %123, label %124, label %93, !llvm.loop !13

124:                                              ; preds = %119
  %125 = add nuw nsw i32 %91, 55
  br label %126

126:                                              ; preds = %124, %89
  %127 = phi i32 [ %22, %89 ], [ %120, %124 ]
  %128 = phi i32 [ %23, %89 ], [ %125, %124 ]
  %129 = getelementptr inbounds nuw i8, ptr %7, i64 12
  %130 = load i32, ptr %129, align 4
  %131 = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %132 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0CB@BAHDAGOF@Words?5inserted?3?5?$CFd?5?$CIunique?3?5?$CFd?$CJ?6@", i32 noundef %128, i32 noundef %130)
  %133 = load i32, ptr %17, align 8
  %134 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BO@MGMADAMK@Table?5size?3?5?$CFd?0?5capacity?3?5?$CFd?6?$AA@", i32 noundef %130, i32 noundef %133)
  %135 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BG@PLKHCFJJ@Input?5checksum?3?5?$CF08x?6?$AA@", i32 noundef %127)
  %136 = getelementptr inbounds nuw i8, ptr %7, i64 28
  %137 = load i32, ptr %136, align 4
  %138 = getelementptr inbounds nuw i8, ptr %7, i64 20
  %139 = getelementptr inbounds nuw i8, ptr %5, i64 255
  %140 = icmp sgt i32 %133, 0
  %141 = icmp sgt i32 %133, 0
  br label %145

142:                                              ; preds = %354
  store i32 %355, ptr %136, align 4
  call void @llvm.lifetime.start.p0(ptr nonnull %6) #18
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(16) %6, i8 0, i64 16, i1 false)
  %143 = getelementptr inbounds nuw i8, ptr %7, i64 16
  %144 = icmp sgt i32 %133, 0
  br label %361

145:                                              ; preds = %126, %354
  %146 = phi i64 [ 0, %126 ], [ %359, %354 ]
  %147 = phi i32 [ 0, %126 ], [ %358, %354 ]
  %148 = phi i32 [ 0, %126 ], [ %357, %354 ]
  %149 = phi i32 [ 0, %126 ], [ %356, %354 ]
  %150 = phi i32 [ %137, %126 ], [ %355, %354 ]
  %151 = getelementptr inbounds nuw ptr, ptr @test_lookups, i64 %146
  %152 = load ptr, ptr %151, align 8
  %153 = load i8, ptr %152, align 1
  %154 = icmp eq i8 %153, 0
  br i1 %154, label %177, label %155

155:                                              ; preds = %145, %155
  %156 = phi i8 [ %163, %155 ], [ %153, %145 ]
  %157 = phi i32 [ %162, %155 ], [ -2128831035, %145 ]
  %158 = phi ptr [ %159, %155 ], [ %152, %145 ]
  %159 = getelementptr inbounds nuw i8, ptr %158, i64 1
  %160 = zext i8 %156 to i32
  %161 = xor i32 %157, %160
  %162 = mul i32 %161, 16777619
  %163 = load i8, ptr %159, align 1
  %164 = icmp eq i8 %163, 0
  br i1 %164, label %165, label %155, !llvm.loop !14

165:                                              ; preds = %155, %165
  %166 = phi i8 [ %173, %165 ], [ %153, %155 ]
  %167 = phi i32 [ %172, %165 ], [ 5381, %155 ]
  %168 = phi ptr [ %170, %165 ], [ %152, %155 ]
  %169 = mul i32 %167, 33
  %170 = getelementptr inbounds nuw i8, ptr %168, i64 1
  %171 = zext i8 %166 to i32
  %172 = add i32 %169, %171
  %173 = load i8, ptr %170, align 1
  %174 = icmp eq i8 %173, 0
  br i1 %174, label %175, label %165, !llvm.loop !15

175:                                              ; preds = %165
  %176 = or i32 %172, 1
  br label %177

177:                                              ; preds = %175, %145
  %178 = phi i32 [ -2128831035, %145 ], [ %162, %175 ]
  %179 = phi i32 [ 5381, %145 ], [ %176, %175 ]
  %180 = add i32 %150, 1
  br i1 %140, label %181, label %237

181:                                              ; preds = %177
  %182 = load ptr, ptr %7, align 8
  %183 = load i32, ptr %138, align 4
  %184 = add i32 %133, %183
  br label %185

185:                                              ; preds = %199, %181
  %186 = phi i32 [ %183, %181 ], [ %190, %199 ]
  %187 = phi i32 [ %178, %181 ], [ %200, %199 ]
  %188 = phi i32 [ 0, %181 ], [ %201, %199 ]
  %189 = urem i32 %187, %133
  %190 = add i32 %186, 1
  %191 = zext nneg i32 %189 to i64
  %192 = getelementptr inbounds nuw %struct.HTEntry, ptr %182, i64 %191
  %193 = getelementptr inbounds nuw i8, ptr %192, i64 12
  %194 = load i32, ptr %193, align 4
  switch i32 %194, label %199 [
    i32 0, label %235
    i32 1, label %195
  ]

195:                                              ; preds = %185
  %196 = load ptr, ptr %192, align 8
  %197 = call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %196, ptr noundef nonnull readonly dereferenceable(1) %152) #18
  %198 = icmp eq i32 %197, 0
  br i1 %198, label %203, label %199

199:                                              ; preds = %195, %185
  %200 = add i32 %189, %179
  %201 = add nuw nsw i32 %188, 1
  %202 = icmp eq i32 %201, %133
  br i1 %202, label %235, label %185, !llvm.loop !16

203:                                              ; preds = %195
  store i32 %190, ptr %138, align 4
  %204 = getelementptr inbounds nuw i8, ptr %192, i64 8
  %205 = load i32, ptr %204, align 8
  %206 = add nsw i32 %147, 1
  br i1 %154, label %224, label %207

207:                                              ; preds = %203, %207
  %208 = phi i8 [ %222, %207 ], [ %153, %203 ]
  %209 = phi ptr [ %212, %207 ], [ %152, %203 ]
  %210 = phi i32 [ %221, %207 ], [ 0, %203 ]
  %211 = phi i32 [ %220, %207 ], [ 0, %203 ]
  %212 = getelementptr inbounds nuw i8, ptr %209, i64 1
  %213 = zext i8 %208 to i32
  %214 = and i32 %210, 255
  %215 = xor i32 %214, %213
  %216 = shl nuw nsw i32 %215, 5
  %217 = xor i32 %216, %211
  %218 = call i32 @llvm.fshl.i32(i32 %217, i32 %217, i32 3)
  %219 = mul i32 %215, -957401568
  %220 = add i32 %218, %219
  %221 = add nuw nsw i32 %210, 1
  %222 = load i8, ptr %212, align 1
  %223 = icmp eq i8 %222, 0
  br i1 %223, label %224, label %207, !llvm.loop !12

224:                                              ; preds = %207, %203
  %225 = phi i32 [ 0, %203 ], [ %220, %207 ]
  %226 = call i32 @llvm.fshl.i32(i32 %205, i32 %205, i32 16)
  %227 = xor i32 %225, %226
  %228 = mul i32 %227, -1640531535
  %229 = lshr i32 %228, 13
  %230 = xor i32 %229, %228
  %231 = mul i32 %230, 1597334677
  %232 = lshr i32 %231, 15
  %233 = xor i32 %149, %232
  %234 = xor i32 %233, %231
  br label %354

235:                                              ; preds = %185, %199
  %236 = phi i32 [ %190, %185 ], [ %184, %199 ]
  store i32 %236, ptr %138, align 4
  br label %237

237:                                              ; preds = %235, %177
  %238 = add nsw i32 %148, 1
  call void @llvm.lifetime.start.p0(ptr nonnull %5) #18
  %239 = call ptr @strncpy(ptr noundef nonnull dereferenceable(1) %5, ptr noundef nonnull dereferenceable(1) %152, i64 noundef 255) #18
  store i8 0, ptr %139, align 1
  %240 = call i64 @strlen(ptr noundef nonnull dereferenceable(1) %5) #18
  %241 = trunc i64 %240 to i32
  %242 = icmp sgt i32 %241, 1
  br i1 %242, label %243, label %279

243:                                              ; preds = %237
  %244 = lshr i64 %240, 1
  %245 = and i64 %240, 2147483647
  %246 = getelementptr i8, ptr %5, i64 %245
  %247 = and i64 %244, 1
  %248 = and i64 %240, 2147483646
  %249 = icmp eq i64 %248, 2
  br i1 %249, label %271, label %250

250:                                              ; preds = %243
  %251 = and i64 %244, 1073741822
  br label %252

252:                                              ; preds = %252, %250
  %253 = phi i64 [ 0, %250 ], [ %266, %252 ]
  %254 = phi i64 [ 0, %250 ], [ %267, %252 ]
  %255 = getelementptr inbounds nuw i8, ptr %5, i64 %253
  %256 = load i8, ptr %255, align 2
  %257 = xor i64 %253, -1
  %258 = getelementptr i8, ptr %246, i64 %257
  %259 = load i8, ptr %258, align 1
  store i8 %259, ptr %255, align 2
  store i8 %256, ptr %258, align 1
  %260 = getelementptr inbounds nuw i8, ptr %5, i64 %253
  %261 = getelementptr inbounds nuw i8, ptr %260, i64 1
  %262 = load i8, ptr %261, align 1
  %263 = xor i64 %253, -2
  %264 = getelementptr i8, ptr %246, i64 %263
  %265 = load i8, ptr %264, align 1
  store i8 %265, ptr %261, align 1
  store i8 %262, ptr %264, align 1
  %266 = add nuw nsw i64 %253, 2
  %267 = add i64 %254, 2
  %268 = icmp eq i64 %267, %251
  br i1 %268, label %269, label %252, !llvm.loop !17

269:                                              ; preds = %252
  %270 = icmp eq i64 %247, 0
  br i1 %270, label %279, label %271

271:                                              ; preds = %269, %243
  %272 = phi i64 [ 0, %243 ], [ %266, %269 ]
  %273 = icmp ne i64 %247, 0
  call void @llvm.assume(i1 %273)
  %274 = getelementptr inbounds nuw i8, ptr %5, i64 %272
  %275 = load i8, ptr %274, align 1
  %276 = xor i64 %272, -1
  %277 = getelementptr i8, ptr %246, i64 %276
  %278 = load i8, ptr %277, align 1
  store i8 %278, ptr %274, align 1
  store i8 %275, ptr %277, align 1
  br label %279

279:                                              ; preds = %271, %269, %237
  %280 = load i8, ptr %5, align 16
  %281 = icmp eq i8 %280, 0
  br i1 %281, label %304, label %282

282:                                              ; preds = %279, %282
  %283 = phi i8 [ %290, %282 ], [ %280, %279 ]
  %284 = phi i32 [ %289, %282 ], [ -2128831035, %279 ]
  %285 = phi ptr [ %286, %282 ], [ %5, %279 ]
  %286 = getelementptr inbounds nuw i8, ptr %285, i64 1
  %287 = zext i8 %283 to i32
  %288 = xor i32 %284, %287
  %289 = mul i32 %288, 16777619
  %290 = load i8, ptr %286, align 1
  %291 = icmp eq i8 %290, 0
  br i1 %291, label %292, label %282, !llvm.loop !14

292:                                              ; preds = %282, %292
  %293 = phi i8 [ %300, %292 ], [ %280, %282 ]
  %294 = phi i32 [ %299, %292 ], [ 5381, %282 ]
  %295 = phi ptr [ %297, %292 ], [ %5, %282 ]
  %296 = mul i32 %294, 33
  %297 = getelementptr inbounds nuw i8, ptr %295, i64 1
  %298 = zext i8 %293 to i32
  %299 = add i32 %296, %298
  %300 = load i8, ptr %297, align 1
  %301 = icmp eq i8 %300, 0
  br i1 %301, label %302, label %292, !llvm.loop !15

302:                                              ; preds = %292
  %303 = or i32 %299, 1
  br label %304

304:                                              ; preds = %302, %279
  %305 = phi i32 [ -2128831035, %279 ], [ %289, %302 ]
  %306 = phi i32 [ 5381, %279 ], [ %303, %302 ]
  %307 = add i32 %150, 2
  br i1 %141, label %308, label %352

308:                                              ; preds = %304
  %309 = load ptr, ptr %7, align 8
  %310 = load i32, ptr %138, align 4
  %311 = add i32 %133, %310
  br label %312

312:                                              ; preds = %326, %308
  %313 = phi i32 [ %310, %308 ], [ %317, %326 ]
  %314 = phi i32 [ %305, %308 ], [ %327, %326 ]
  %315 = phi i32 [ 0, %308 ], [ %328, %326 ]
  %316 = urem i32 %314, %133
  %317 = add i32 %313, 1
  %318 = zext nneg i32 %316 to i64
  %319 = getelementptr inbounds nuw %struct.HTEntry, ptr %309, i64 %318
  %320 = getelementptr inbounds nuw i8, ptr %319, i64 12
  %321 = load i32, ptr %320, align 4
  switch i32 %321, label %326 [
    i32 0, label %350
    i32 1, label %322
  ]

322:                                              ; preds = %312
  %323 = load ptr, ptr %319, align 8
  %324 = call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %323, ptr noundef nonnull readonly dereferenceable(1) %5) #18
  %325 = icmp eq i32 %324, 0
  br i1 %325, label %330, label %326

326:                                              ; preds = %322, %312
  %327 = add i32 %316, %306
  %328 = add nuw nsw i32 %315, 1
  %329 = icmp eq i32 %328, %133
  br i1 %329, label %350, label %312, !llvm.loop !16

330:                                              ; preds = %322
  store i32 %317, ptr %138, align 4
  br i1 %281, label %352, label %331

331:                                              ; preds = %330, %331
  %332 = phi i8 [ %346, %331 ], [ %280, %330 ]
  %333 = phi ptr [ %336, %331 ], [ %5, %330 ]
  %334 = phi i32 [ %345, %331 ], [ 0, %330 ]
  %335 = phi i32 [ %344, %331 ], [ 0, %330 ]
  %336 = getelementptr inbounds nuw i8, ptr %333, i64 1
  %337 = zext i8 %332 to i32
  %338 = and i32 %334, 255
  %339 = xor i32 %338, %337
  %340 = shl nuw nsw i32 %339, 5
  %341 = xor i32 %340, %335
  %342 = call i32 @llvm.fshl.i32(i32 %341, i32 %341, i32 3)
  %343 = mul i32 %339, -957401568
  %344 = add i32 %342, %343
  %345 = add nuw nsw i32 %334, 1
  %346 = load i8, ptr %336, align 1
  %347 = icmp eq i8 %346, 0
  br i1 %347, label %348, label %331, !llvm.loop !12

348:                                              ; preds = %331
  %349 = xor i32 %344, %149
  br label %352

350:                                              ; preds = %312, %326
  %351 = phi i32 [ %317, %312 ], [ %311, %326 ]
  store i32 %351, ptr %138, align 4
  br label %352

352:                                              ; preds = %330, %348, %350, %304
  %353 = phi i32 [ %149, %350 ], [ %149, %304 ], [ %149, %330 ], [ %349, %348 ]
  call void @llvm.lifetime.end.p0(ptr nonnull %5) #18
  br label %354

354:                                              ; preds = %352, %224
  %355 = phi i32 [ %180, %224 ], [ %307, %352 ]
  %356 = phi i32 [ %234, %224 ], [ %353, %352 ]
  %357 = phi i32 [ %148, %224 ], [ %238, %352 ]
  %358 = phi i32 [ %206, %224 ], [ %147, %352 ]
  %359 = add nuw nsw i64 %146, 1
  %360 = icmp eq i64 %359, 15
  br i1 %360, label %142, label %145, !llvm.loop !18

361:                                              ; preds = %142, %419
  %362 = phi i32 [ %130, %142 ], [ %420, %419 ]
  %363 = phi i64 [ 0, %142 ], [ %422, %419 ]
  %364 = phi i32 [ 0, %142 ], [ %421, %419 ]
  %365 = getelementptr inbounds nuw ptr, ptr @__const.main.to_delete, i64 %363
  %366 = load ptr, ptr %365, align 8
  %367 = load i8, ptr %366, align 1
  %368 = icmp eq i8 %367, 0
  br i1 %368, label %391, label %369

369:                                              ; preds = %361, %369
  %370 = phi i8 [ %377, %369 ], [ %367, %361 ]
  %371 = phi i32 [ %376, %369 ], [ -2128831035, %361 ]
  %372 = phi ptr [ %373, %369 ], [ %366, %361 ]
  %373 = getelementptr inbounds nuw i8, ptr %372, i64 1
  %374 = zext i8 %370 to i32
  %375 = xor i32 %371, %374
  %376 = mul i32 %375, 16777619
  %377 = load i8, ptr %373, align 1
  %378 = icmp eq i8 %377, 0
  br i1 %378, label %379, label %369, !llvm.loop !14

379:                                              ; preds = %369, %379
  %380 = phi i8 [ %387, %379 ], [ %367, %369 ]
  %381 = phi i32 [ %386, %379 ], [ 5381, %369 ]
  %382 = phi ptr [ %384, %379 ], [ %366, %369 ]
  %383 = mul i32 %381, 33
  %384 = getelementptr inbounds nuw i8, ptr %382, i64 1
  %385 = zext i8 %380 to i32
  %386 = add i32 %383, %385
  %387 = load i8, ptr %384, align 1
  %388 = icmp eq i8 %387, 0
  br i1 %388, label %389, label %379, !llvm.loop !15

389:                                              ; preds = %379
  %390 = or i32 %386, 1
  br label %391

391:                                              ; preds = %389, %361
  %392 = phi i32 [ -2128831035, %361 ], [ %376, %389 ]
  %393 = phi i32 [ 5381, %361 ], [ %390, %389 ]
  br i1 %144, label %394, label %419

394:                                              ; preds = %391
  %395 = load ptr, ptr %7, align 8
  br label %396

396:                                              ; preds = %408, %394
  %397 = phi i32 [ 0, %394 ], [ %410, %408 ]
  %398 = phi i32 [ %392, %394 ], [ %409, %408 ]
  %399 = urem i32 %398, %133
  %400 = zext nneg i32 %399 to i64
  %401 = getelementptr inbounds nuw %struct.HTEntry, ptr %395, i64 %400
  %402 = getelementptr inbounds nuw i8, ptr %401, i64 12
  %403 = load i32, ptr %402, align 4
  switch i32 %403, label %408 [
    i32 0, label %419
    i32 1, label %404
  ]

404:                                              ; preds = %396
  %405 = load ptr, ptr %401, align 8
  %406 = call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %405, ptr noundef nonnull readonly dereferenceable(1) %366) #18
  %407 = icmp eq i32 %406, 0
  br i1 %407, label %412, label %408

408:                                              ; preds = %404, %396
  %409 = add i32 %399, %393
  %410 = add nuw nsw i32 %397, 1
  %411 = icmp eq i32 %410, %133
  br i1 %411, label %419, label %396, !llvm.loop !19

412:                                              ; preds = %404
  %413 = getelementptr inbounds nuw i8, ptr %401, i64 12
  call void @free(ptr noundef nonnull %405)
  store ptr null, ptr %401, align 8
  store i32 2, ptr %413, align 4
  %414 = add nsw i32 %362, -1
  store i32 %414, ptr %129, align 4
  %415 = load i32, ptr %143, align 8
  %416 = add nsw i32 %415, 1
  store i32 %416, ptr %143, align 8
  %417 = getelementptr inbounds nuw i32, ptr %6, i64 %363
  store i32 1, ptr %417, align 4
  %418 = add nsw i32 %364, 1
  br label %419

419:                                              ; preds = %408, %396, %391, %412
  %420 = phi i32 [ %414, %412 ], [ %362, %391 ], [ %362, %396 ], [ %362, %408 ]
  %421 = phi i32 [ %418, %412 ], [ %364, %391 ], [ %364, %396 ], [ %364, %408 ]
  %422 = add nuw nsw i64 %363, 1
  %423 = icmp eq i64 %422, 4
  br i1 %423, label %428, label %361, !llvm.loop !20

424:                                              ; preds = %491
  %425 = add i32 %355, 4
  store i32 %425, ptr %136, align 4
  %426 = load i32, ptr %6, align 16
  %427 = icmp eq i32 %426, 0
  br i1 %427, label %496, label %495

428:                                              ; preds = %419, %491
  %429 = phi i64 [ %493, %491 ], [ 0, %419 ]
  %430 = phi i32 [ %492, %491 ], [ 1, %419 ]
  %431 = getelementptr inbounds nuw ptr, ptr @__const.main.to_delete, i64 %429
  %432 = load ptr, ptr %431, align 8
  %433 = load i8, ptr %432, align 1
  %434 = icmp eq i8 %433, 0
  br i1 %434, label %457, label %435

435:                                              ; preds = %428, %435
  %436 = phi i8 [ %443, %435 ], [ %433, %428 ]
  %437 = phi i32 [ %442, %435 ], [ -2128831035, %428 ]
  %438 = phi ptr [ %439, %435 ], [ %432, %428 ]
  %439 = getelementptr inbounds nuw i8, ptr %438, i64 1
  %440 = zext i8 %436 to i32
  %441 = xor i32 %437, %440
  %442 = mul i32 %441, 16777619
  %443 = load i8, ptr %439, align 1
  %444 = icmp eq i8 %443, 0
  br i1 %444, label %445, label %435, !llvm.loop !14

445:                                              ; preds = %435, %445
  %446 = phi i8 [ %453, %445 ], [ %433, %435 ]
  %447 = phi i32 [ %452, %445 ], [ 5381, %435 ]
  %448 = phi ptr [ %450, %445 ], [ %432, %435 ]
  %449 = mul i32 %447, 33
  %450 = getelementptr inbounds nuw i8, ptr %448, i64 1
  %451 = zext i8 %446 to i32
  %452 = add i32 %449, %451
  %453 = load i8, ptr %450, align 1
  %454 = icmp eq i8 %453, 0
  br i1 %454, label %455, label %445, !llvm.loop !15

455:                                              ; preds = %445
  %456 = or i32 %452, 1
  br label %457

457:                                              ; preds = %455, %428
  %458 = phi i32 [ -2128831035, %428 ], [ %442, %455 ]
  %459 = phi i32 [ 5381, %428 ], [ %456, %455 ]
  br i1 %144, label %460, label %491

460:                                              ; preds = %457
  %461 = load ptr, ptr %7, align 8
  %462 = load i32, ptr %138, align 4
  %463 = add i32 %133, %462
  br label %464

464:                                              ; preds = %478, %460
  %465 = phi i32 [ %462, %460 ], [ %469, %478 ]
  %466 = phi i32 [ %458, %460 ], [ %479, %478 ]
  %467 = phi i32 [ 0, %460 ], [ %480, %478 ]
  %468 = urem i32 %466, %133
  %469 = add i32 %465, 1
  %470 = zext nneg i32 %468 to i64
  %471 = getelementptr inbounds nuw %struct.HTEntry, ptr %461, i64 %470
  %472 = getelementptr inbounds nuw i8, ptr %471, i64 12
  %473 = load i32, ptr %472, align 4
  switch i32 %473, label %478 [
    i32 0, label %489
    i32 1, label %474
  ]

474:                                              ; preds = %464
  %475 = load ptr, ptr %471, align 8
  %476 = call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %475, ptr noundef nonnull readonly dereferenceable(1) %432) #18
  %477 = icmp eq i32 %476, 0
  br i1 %477, label %482, label %478

478:                                              ; preds = %474, %464
  %479 = add i32 %468, %459
  %480 = add nuw nsw i32 %467, 1
  %481 = icmp eq i32 %480, %133
  br i1 %481, label %489, label %464, !llvm.loop !16

482:                                              ; preds = %474
  store i32 %469, ptr %138, align 4
  %483 = getelementptr inbounds nuw i32, ptr %6, i64 %429
  %484 = load i32, ptr %483, align 4
  %485 = icmp eq i32 %484, 0
  br i1 %485, label %491, label %486

486:                                              ; preds = %482
  %487 = call ptr @__acrt_iob_func(i32 noundef 2) #18
  %488 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %487, ptr noundef nonnull @"??_C@_0CH@EIIFJFPB@FAIL?3?5?8?$CFs?8?5still?5found?5after?5del@", ptr noundef nonnull %432) #18
  br label %491

489:                                              ; preds = %464, %478
  %490 = phi i32 [ %469, %464 ], [ %463, %478 ]
  store i32 %490, ptr %138, align 4
  br label %491

491:                                              ; preds = %489, %457, %486, %482
  %492 = phi i32 [ 0, %486 ], [ %430, %482 ], [ %430, %457 ], [ %430, %489 ]
  %493 = add nuw nsw i64 %429, 1
  %494 = icmp eq i64 %493, 4
  br i1 %494, label %424, label %428, !llvm.loop !21

495:                                              ; preds = %424
  call fastcc void @ht_insert(ptr noundef %7, ptr noundef nonnull @"??_C@_05IAEKHIAN@alpha?$AA@", i32 noundef 9999)
  br label %496

496:                                              ; preds = %424, %495
  %497 = getelementptr inbounds nuw i8, ptr %6, i64 4
  %498 = load i32, ptr %497, align 4
  %499 = icmp eq i32 %498, 0
  br i1 %499, label %501, label %500

500:                                              ; preds = %496
  call fastcc void @ht_insert(ptr noundef %7, ptr noundef nonnull @"??_C@_07BCCGMNOK@charlie?$AA@", i32 noundef 10000)
  br label %501

501:                                              ; preds = %500, %496
  %502 = getelementptr inbounds nuw i8, ptr %6, i64 8
  %503 = load i32, ptr %502, align 8
  %504 = icmp eq i32 %503, 0
  br i1 %504, label %506, label %505

505:                                              ; preds = %501
  call fastcc void @ht_insert(ptr noundef %7, ptr noundef nonnull @"??_C@_04DOALKIP@golf?$AA@", i32 noundef 10001)
  br label %506

506:                                              ; preds = %505, %501
  %507 = getelementptr inbounds nuw i8, ptr %6, i64 12
  %508 = load i32, ptr %507, align 4
  %509 = icmp eq i32 %508, 0
  br i1 %509, label %510, label %511

510:                                              ; preds = %511, %506
  br label %525

511:                                              ; preds = %506
  call fastcc void @ht_insert(ptr noundef %7, ptr noundef nonnull @"??_C@_06CGCFEEDE@banana?$AA@", i32 noundef 10002)
  br label %510

512:                                              ; preds = %597
  %513 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BG@ELBGGNKF@Hits?3?5?$CFd?0?5Misses?3?5?$CFd?6?$AA@", i32 noundef %358, i32 noundef %357)
  %514 = icmp ne i32 %492, 0
  %515 = select i1 %514, ptr @"??_C@_03ICICOMAL@yes?$AA@", ptr @"??_C@_02KAJCLHKP@no?$AA@"
  %516 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0CC@NIKOCKLC@Deleted?3?5?$CFd?0?5Delete?5verified?3?5?$CFs@", i32 noundef %421, ptr noundef nonnull %515)
  %517 = icmp ne i32 %598, 0
  %518 = select i1 %517, ptr @"??_C@_03ICICOMAL@yes?$AA@", ptr @"??_C@_02KAJCLHKP@no?$AA@"
  %519 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BI@EIGPOAFK@Re?9insert?5verified?3?5?$CFs?6?$AA@", ptr noundef nonnull %518)
  %520 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BH@JIKJOHDO@Lookup?5checksum?3?5?$CF08x?6?$AA@", i32 noundef %356)
  %521 = load i32, ptr %138, align 4
  %522 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BC@JJCOHFPB@Total?5probes?3?5?$CFu?6?$AA@", i32 noundef %521)
  %523 = load i32, ptr %129, align 4
  %524 = icmp eq i32 %523, %130
  br i1 %524, label %606, label %603

525:                                              ; preds = %510, %597
  %526 = phi i64 [ %599, %597 ], [ 0, %510 ]
  %527 = phi ptr [ %601, %597 ], [ @"??_C@_05IAEKHIAN@alpha?$AA@", %510 ]
  %528 = phi i32 [ %598, %597 ], [ 1, %510 ]
  %529 = getelementptr inbounds nuw i32, ptr %6, i64 %526
  %530 = load i32, ptr %529, align 4
  %531 = icmp eq i32 %530, 0
  br i1 %531, label %597, label %532

532:                                              ; preds = %525
  %533 = load i8, ptr %527, align 1
  %534 = icmp eq i8 %533, 0
  br i1 %534, label %557, label %535

535:                                              ; preds = %532, %535
  %536 = phi i8 [ %543, %535 ], [ %533, %532 ]
  %537 = phi i32 [ %542, %535 ], [ -2128831035, %532 ]
  %538 = phi ptr [ %539, %535 ], [ %527, %532 ]
  %539 = getelementptr inbounds nuw i8, ptr %538, i64 1
  %540 = zext i8 %536 to i32
  %541 = xor i32 %537, %540
  %542 = mul i32 %541, 16777619
  %543 = load i8, ptr %539, align 1
  %544 = icmp eq i8 %543, 0
  br i1 %544, label %545, label %535, !llvm.loop !14

545:                                              ; preds = %535, %545
  %546 = phi i8 [ %553, %545 ], [ %533, %535 ]
  %547 = phi i32 [ %552, %545 ], [ 5381, %535 ]
  %548 = phi ptr [ %550, %545 ], [ %527, %535 ]
  %549 = mul i32 %547, 33
  %550 = getelementptr inbounds nuw i8, ptr %548, i64 1
  %551 = zext i8 %546 to i32
  %552 = add i32 %549, %551
  %553 = load i8, ptr %550, align 1
  %554 = icmp eq i8 %553, 0
  br i1 %554, label %555, label %545, !llvm.loop !15

555:                                              ; preds = %545
  %556 = or i32 %552, 1
  br label %557

557:                                              ; preds = %555, %532
  %558 = phi i32 [ -2128831035, %532 ], [ %542, %555 ]
  %559 = phi i32 [ 5381, %532 ], [ %556, %555 ]
  %560 = load i32, ptr %17, align 8
  %561 = load i32, ptr %136, align 4
  %562 = add i32 %561, 1
  store i32 %562, ptr %136, align 4
  %563 = icmp sgt i32 %560, 0
  br i1 %563, label %564, label %594

564:                                              ; preds = %557
  %565 = load ptr, ptr %7, align 8
  %566 = load i32, ptr %138, align 4
  %567 = add i32 %560, %566
  br label %568

568:                                              ; preds = %582, %564
  %569 = phi i32 [ %566, %564 ], [ %573, %582 ]
  %570 = phi i32 [ %558, %564 ], [ %583, %582 ]
  %571 = phi i32 [ 0, %564 ], [ %584, %582 ]
  %572 = urem i32 %570, %560
  %573 = add i32 %569, 1
  %574 = zext nneg i32 %572 to i64
  %575 = getelementptr inbounds nuw %struct.HTEntry, ptr %565, i64 %574
  %576 = getelementptr inbounds nuw i8, ptr %575, i64 12
  %577 = load i32, ptr %576, align 4
  switch i32 %577, label %582 [
    i32 0, label %592
    i32 1, label %578
  ]

578:                                              ; preds = %568
  %579 = load ptr, ptr %575, align 8
  %580 = call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %579, ptr noundef nonnull readonly dereferenceable(1) %527) #18
  %581 = icmp eq i32 %580, 0
  br i1 %581, label %586, label %582

582:                                              ; preds = %578, %568
  %583 = add i32 %572, %559
  %584 = add nuw nsw i32 %571, 1
  %585 = icmp eq i32 %584, %560
  br i1 %585, label %592, label %568, !llvm.loop !16

586:                                              ; preds = %578
  store i32 %573, ptr %138, align 4
  %587 = getelementptr inbounds nuw i8, ptr %575, i64 8
  %588 = load i32, ptr %587, align 8
  %589 = add nuw nsw i64 %526, 9999
  %590 = zext i32 %588 to i64
  %591 = icmp eq i64 %589, %590
  br i1 %591, label %597, label %594

592:                                              ; preds = %568, %582
  %593 = phi i32 [ %573, %568 ], [ %567, %582 ]
  store i32 %593, ptr %138, align 4
  br label %594

594:                                              ; preds = %592, %557, %586
  %595 = call ptr @__acrt_iob_func(i32 noundef 2) #18
  %596 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %595, ptr noundef nonnull @"??_C@_0CJ@HDNIDCA@FAIL?3?5?8?$CFs?8?5not?5found?5after?5re?9in@", ptr noundef nonnull %527) #18
  br label %597

597:                                              ; preds = %586, %594, %525
  %598 = phi i32 [ %528, %525 ], [ 0, %594 ], [ %528, %586 ]
  %599 = add nuw nsw i64 %526, 1
  %600 = getelementptr inbounds nuw ptr, ptr @__const.main.to_delete, i64 %599
  %601 = load ptr, ptr %600, align 8
  %602 = icmp eq i64 %599, 4
  br i1 %602, label %512, label %525, !llvm.loop !22

603:                                              ; preds = %512
  %604 = call ptr @__acrt_iob_func(i32 noundef 2) #18
  %605 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %604, ptr noundef nonnull @"??_C@_0DK@IJMNHKLC@FAIL?3?5size?5mismatch?5after?5operat@", i32 noundef %523, i32 noundef %130) #18
  br label %608

606:                                              ; preds = %512
  %607 = and i1 %514, %517
  br i1 %607, label %609, label %608

608:                                              ; preds = %603, %606
  br label %609

609:                                              ; preds = %606, %608
  %610 = phi ptr [ @str.2, %608 ], [ @str.1, %606 ]
  %611 = phi i32 [ 1, %608 ], [ 0, %606 ]
  %612 = call i32 @puts(ptr nonnull dereferenceable(1) %610)
  %613 = load i32, ptr %17, align 8
  %614 = icmp sgt i32 %613, 0
  %615 = load ptr, ptr %7, align 8
  br i1 %614, label %616, label %629

616:                                              ; preds = %609
  %617 = zext nneg i32 %613 to i64
  br label %618

618:                                              ; preds = %616, %626
  %619 = phi i64 [ %627, %626 ], [ 0, %616 ]
  %620 = getelementptr inbounds nuw %struct.HTEntry, ptr %615, i64 %619
  %621 = getelementptr inbounds nuw i8, ptr %620, i64 12
  %622 = load i32, ptr %621, align 4
  %623 = icmp eq i32 %622, 1
  br i1 %623, label %624, label %626

624:                                              ; preds = %618
  %625 = load ptr, ptr %620, align 8
  call void @free(ptr noundef %625)
  br label %626

626:                                              ; preds = %624, %618
  %627 = add nuw nsw i64 %619, 1
  %628 = icmp samesign ult i64 %627, %617
  br i1 %628, label %618, label %629, !llvm.loop !23

629:                                              ; preds = %626, %609
  call void @free(ptr noundef %615)
  call void @free(ptr noundef nonnull %7)
  call void @llvm.lifetime.end.p0(ptr nonnull %6) #18
  call void @llvm.lifetime.end.p0(ptr nonnull %3) #18
  br label %630

630:                                              ; preds = %629, %13
  %631 = phi i32 [ %611, %629 ], [ 1, %13 ]
  ret i32 %631
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #1

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @fprintf(ptr noundef %0, ptr noundef %1, ...) local_unnamed_addr #2 comdat {
  %3 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %3) #18
  call void @llvm.va_start.p0(ptr nonnull %3)
  %4 = load ptr, ptr %3, align 8
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %0, ptr noundef %1, ptr noundef null, ptr noundef %4) #18
  call void @llvm.va_end.p0(ptr nonnull %3)
  call void @llvm.lifetime.end.p0(ptr nonnull %3) #18
  ret i32 %7
}

declare dso_local ptr @__acrt_iob_func(i32 noundef) local_unnamed_addr #3

declare dso_local i32 @_isatty(i32 noundef) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare dso_local noundef ptr @fgets(ptr noundef writeonly, i32 noundef, ptr noundef captures(none)) local_unnamed_addr #4

; Function Attrs: nounwind memory(readwrite, target_mem0: none, target_mem1: none) uwtable
define internal fastcc void @ht_insert(ptr noundef nonnull captures(none) %0, ptr noundef readonly captures(none) %1, i32 noundef %2) unnamed_addr #5 {
  %4 = getelementptr inbounds nuw i8, ptr %0, i64 12
  %5 = load i32, ptr %4, align 4
  %6 = getelementptr inbounds nuw i8, ptr %0, i64 16
  %7 = load i32, ptr %6, align 8
  %8 = add nsw i32 %7, %5
  %9 = mul nsw i32 %8, 10
  %10 = getelementptr inbounds nuw i8, ptr %0, i64 8
  %11 = load i32, ptr %10, align 8
  %12 = mul nsw i32 %11, 7
  %13 = icmp slt i32 %9, %12
  br i1 %13, label %40, label %14

14:                                               ; preds = %3
  %15 = shl nsw i32 %11, 1
  %16 = load ptr, ptr %0, align 8
  %17 = sext i32 %15 to i64
  %18 = tail call noalias ptr @calloc(i64 noundef %17, i64 noundef 16) #17
  store ptr %18, ptr %0, align 8
  %19 = icmp eq ptr %18, null
  br i1 %19, label %39, label %20

20:                                               ; preds = %14
  store i32 %15, ptr %10, align 8
  store i32 0, ptr %4, align 4
  store i32 0, ptr %6, align 8
  %21 = icmp sgt i32 %11, 0
  br i1 %21, label %22, label %24

22:                                               ; preds = %20
  %23 = zext nneg i32 %11 to i64
  br label %25

24:                                               ; preds = %36, %20
  tail call void @free(ptr noundef %16)
  br label %40

25:                                               ; preds = %22, %36
  %26 = phi i64 [ 0, %22 ], [ %37, %36 ]
  %27 = getelementptr inbounds nuw %struct.HTEntry, ptr %16, i64 %26
  %28 = getelementptr inbounds nuw i8, ptr %27, i64 12
  %29 = load i32, ptr %28, align 4
  %30 = icmp eq i32 %29, 1
  br i1 %30, label %31, label %36

31:                                               ; preds = %25
  %32 = getelementptr inbounds nuw i8, ptr %27, i64 8
  %33 = load i32, ptr %32, align 8
  %34 = load ptr, ptr %27, align 8
  tail call fastcc void @ht_insert(ptr noundef nonnull %0, ptr noundef %34, i32 noundef %33)
  %35 = load ptr, ptr %27, align 8
  tail call void @free(ptr noundef %35)
  br label %36

36:                                               ; preds = %31, %25
  %37 = add nuw nsw i64 %26, 1
  %38 = icmp eq i64 %37, %23
  br i1 %38, label %24, label %25, !llvm.loop !24

39:                                               ; preds = %14
  store ptr %16, ptr %0, align 8
  br label %126

40:                                               ; preds = %24, %3
  %41 = load i8, ptr %1, align 1
  %42 = icmp eq i8 %41, 0
  br i1 %42, label %65, label %43

43:                                               ; preds = %40, %43
  %44 = phi i8 [ %51, %43 ], [ %41, %40 ]
  %45 = phi i32 [ %50, %43 ], [ -2128831035, %40 ]
  %46 = phi ptr [ %47, %43 ], [ %1, %40 ]
  %47 = getelementptr inbounds nuw i8, ptr %46, i64 1
  %48 = zext i8 %44 to i32
  %49 = xor i32 %45, %48
  %50 = mul i32 %49, 16777619
  %51 = load i8, ptr %47, align 1
  %52 = icmp eq i8 %51, 0
  br i1 %52, label %53, label %43, !llvm.loop !14

53:                                               ; preds = %43, %53
  %54 = phi i8 [ %61, %53 ], [ %41, %43 ]
  %55 = phi i32 [ %60, %53 ], [ 5381, %43 ]
  %56 = phi ptr [ %58, %53 ], [ %1, %43 ]
  %57 = mul i32 %55, 33
  %58 = getelementptr inbounds nuw i8, ptr %56, i64 1
  %59 = zext i8 %54 to i32
  %60 = add i32 %57, %59
  %61 = load i8, ptr %58, align 1
  %62 = icmp eq i8 %61, 0
  br i1 %62, label %63, label %53, !llvm.loop !15

63:                                               ; preds = %53
  %64 = or i32 %60, 1
  br label %65

65:                                               ; preds = %63, %40
  %66 = phi i32 [ -2128831035, %40 ], [ %50, %63 ]
  %67 = phi i32 [ 5381, %40 ], [ %64, %63 ]
  %68 = load i32, ptr %10, align 8
  %69 = icmp sgt i32 %68, 0
  br i1 %69, label %70, label %126

70:                                               ; preds = %65
  %71 = getelementptr inbounds nuw i8, ptr %0, i64 20
  %72 = load ptr, ptr %0, align 8
  %73 = load i32, ptr %71, align 4
  br label %74

74:                                               ; preds = %70, %122
  %75 = phi i32 [ %73, %70 ], [ %80, %122 ]
  %76 = phi i32 [ %66, %70 ], [ %123, %122 ]
  %77 = phi i32 [ -1, %70 ], [ %111, %122 ]
  %78 = phi i32 [ 0, %70 ], [ %124, %122 ]
  %79 = urem i32 %76, %68
  %80 = add i32 %75, 1
  store i32 %80, ptr %71, align 4
  %81 = zext nneg i32 %79 to i64
  %82 = getelementptr inbounds nuw %struct.HTEntry, ptr %72, i64 %81
  %83 = getelementptr inbounds nuw i8, ptr %82, i64 12
  %84 = load i32, ptr %83, align 4
  %85 = icmp eq i32 %84, 0
  br i1 %85, label %86, label %107

86:                                               ; preds = %74
  %87 = icmp slt i32 %77, 0
  %88 = select i1 %87, i32 %79, i32 %77
  %89 = sext i32 %88 to i64
  %90 = getelementptr inbounds %struct.HTEntry, ptr %72, i64 %89
  %91 = getelementptr inbounds nuw i8, ptr %90, i64 12
  %92 = load i32, ptr %91, align 4
  %93 = icmp eq i32 %92, 2
  br i1 %93, label %94, label %97

94:                                               ; preds = %86
  %95 = load i32, ptr %6, align 8
  %96 = add nsw i32 %95, -1
  store i32 %96, ptr %6, align 8
  br label %97

97:                                               ; preds = %94, %86
  %98 = tail call ptr @strdup(ptr noundef nonnull %1)
  store ptr %98, ptr %90, align 8
  %99 = icmp eq ptr %98, null
  br i1 %99, label %126, label %100

100:                                              ; preds = %97
  %101 = getelementptr inbounds nuw i8, ptr %90, i64 8
  store i32 %2, ptr %101, align 8
  store i32 1, ptr %91, align 4
  %102 = load i32, ptr %4, align 4
  %103 = add nsw i32 %102, 1
  store i32 %103, ptr %4, align 4
  %104 = getelementptr inbounds nuw i8, ptr %0, i64 24
  %105 = load i32, ptr %104, align 8
  %106 = add i32 %105, 1
  store i32 %106, ptr %104, align 8
  br label %126

107:                                              ; preds = %74
  %108 = icmp eq i32 %84, 2
  %109 = icmp slt i32 %77, 0
  %110 = select i1 %108, i1 %109, i1 false
  %111 = select i1 %110, i32 %79, i32 %77
  %112 = icmp eq i32 %84, 1
  br i1 %112, label %113, label %122

113:                                              ; preds = %107
  %114 = load ptr, ptr %82, align 8
  %115 = tail call i32 @strcmp(ptr noundef nonnull dereferenceable(1) %114, ptr noundef nonnull dereferenceable(1) %1) #18
  %116 = icmp eq i32 %115, 0
  br i1 %116, label %117, label %122

117:                                              ; preds = %113
  %118 = getelementptr inbounds nuw i8, ptr %82, i64 8
  store i32 %2, ptr %118, align 8
  %119 = getelementptr inbounds nuw i8, ptr %0, i64 24
  %120 = load i32, ptr %119, align 8
  %121 = add i32 %120, 1
  store i32 %121, ptr %119, align 8
  br label %126

122:                                              ; preds = %113, %107
  %123 = add i32 %79, %67
  %124 = add nuw nsw i32 %78, 1
  %125 = icmp eq i32 %124, %68
  br i1 %125, label %126, label %74, !llvm.loop !25

126:                                              ; preds = %122, %65, %117, %97, %100, %39
  ret void
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(ptr captures(none)) #1

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @printf(ptr noundef %0, ...) local_unnamed_addr #2 comdat {
  %2 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %2) #18
  call void @llvm.va_start.p0(ptr nonnull %2)
  %3 = load ptr, ptr %2, align 8
  %4 = call ptr @__acrt_iob_func(i32 noundef 1) #18
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %4, ptr noundef %0, ptr noundef null, ptr noundef %3) #18
  call void @llvm.va_end.p0(ptr nonnull %2)
  call void @llvm.lifetime.end.p0(ptr nonnull %2) #18
  ret i32 %7
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare dso_local ptr @strncpy(ptr noalias noundef returned writeonly, ptr noalias noundef readonly captures(none), i64 noundef) local_unnamed_addr #6

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #7

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #8

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #8

; Function Attrs: noinline nounwind uwtable
define linkonce_odr dso_local ptr @__local_stdio_printf_options() local_unnamed_addr #9 comdat {
  ret ptr @__local_stdio_printf_options._OptionsStorage
}

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,zeroed") allocsize(0,1) memory(inaccessiblemem: readwrite)
declare dso_local noalias noundef ptr @calloc(i64 noundef, i64 noundef) local_unnamed_addr #10

; Function Attrs: mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite)
declare dso_local void @free(ptr allocptr noundef captures(none)) local_unnamed_addr #11

declare dso_local i32 @__stdio_common_vfprintf(i64 noundef, ptr noundef, ptr noundef, ptr noundef, ptr noundef) local_unnamed_addr #3

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: read)
declare dso_local i64 @strlen(ptr noundef captures(none)) local_unnamed_addr #12

; Function Attrs: mustprogress nofree nounwind willreturn memory(argmem: readwrite, inaccessiblemem: readwrite)
declare dso_local noalias ptr @strdup(ptr noundef readonly captures(none)) local_unnamed_addr #13

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: read)
declare dso_local i32 @strcmp(ptr noundef captures(none), ptr noundef captures(none)) local_unnamed_addr #12

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #14

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #14

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.fshl.i32(i32, i32, i32) #15

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef) #16

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { inlinehint nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind memory(readwrite, target_mem0: none, target_mem1: none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #8 = { mustprogress nocallback nofree nosync nounwind willreturn }
attributes #9 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #10 = { mustprogress nofree nounwind willreturn allockind("alloc,zeroed") allocsize(0,1) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #11 = { mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #12 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: read) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #13 = { mustprogress nofree nounwind willreturn memory(argmem: readwrite, inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #14 = { nofree nounwind }
attributes #15 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #16 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #17 = { allocsize(0,1) }
attributes #18 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 22.1.2 (https://github.com/llvm/llvm-project 1ab49a973e210e97d61e5db6557180dcb92c3e98)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "hashtable.c", directory: "C:\\Users\\Treverm\\Documents\\Claude Code\\llvm-obfuscator\\eval\\benchmarks")
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
!19 = distinct !{!19, !9}
!20 = distinct !{!20, !9}
!21 = distinct !{!21, !9}
!22 = distinct !{!22, !9}
!23 = distinct !{!23, !9}
!24 = distinct !{!24, !9}
!25 = distinct !{!25, !9}
