/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse lefyyparse
#define yylex   lefyylex
#define yyerror lefyyerror
#define yylval  lefyylval
#define yychar  lefyychar
#define yydebug lefyydebug
#define yynerrs lefyynerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     K_HISTORY = 258,
     K_ABUT = 259,
     K_ABUTMENT = 260,
     K_ACTIVE = 261,
     K_ANALOG = 262,
     K_ARRAY = 263,
     K_AREA = 264,
     K_BLOCK = 265,
     K_BOTTOMLEFT = 266,
     K_BOTTOMRIGHT = 267,
     K_BY = 268,
     K_CAPACITANCE = 269,
     K_CAPMULTIPLIER = 270,
     K_CLASS = 271,
     K_CLOCK = 272,
     K_CLOCKTYPE = 273,
     K_COLUMNMAJOR = 274,
     K_DESIGNRULEWIDTH = 275,
     K_INFLUENCE = 276,
     K_CORE = 277,
     K_CORNER = 278,
     K_COVER = 279,
     K_CPERSQDIST = 280,
     K_CURRENT = 281,
     K_CURRENTSOURCE = 282,
     K_CUT = 283,
     K_DEFAULT = 284,
     K_DATABASE = 285,
     K_DATA = 286,
     K_DIELECTRIC = 287,
     K_DIRECTION = 288,
     K_DO = 289,
     K_EDGECAPACITANCE = 290,
     K_EEQ = 291,
     K_END = 292,
     K_ENDCAP = 293,
     K_FALL = 294,
     K_FALLCS = 295,
     K_FALLT0 = 296,
     K_FALLSATT1 = 297,
     K_FALLRS = 298,
     K_FALLSATCUR = 299,
     K_FALLTHRESH = 300,
     K_FEEDTHRU = 301,
     K_FIXED = 302,
     K_FOREIGN = 303,
     K_FROMPIN = 304,
     K_GENERATE = 305,
     K_GENERATOR = 306,
     K_GROUND = 307,
     K_HEIGHT = 308,
     K_HORIZONTAL = 309,
     K_INOUT = 310,
     K_INPUT = 311,
     K_INPUTNOISEMARGIN = 312,
     K_COMPONENTPIN = 313,
     K_INTRINSIC = 314,
     K_INVERT = 315,
     K_IRDROP = 316,
     K_ITERATE = 317,
     K_IV_TABLES = 318,
     K_LAYER = 319,
     K_LEAKAGE = 320,
     K_LEQ = 321,
     K_LIBRARY = 322,
     K_MACRO = 323,
     K_MATCH = 324,
     K_MAXDELAY = 325,
     K_MAXLOAD = 326,
     K_METALOVERHANG = 327,
     K_MILLIAMPS = 328,
     K_MILLIWATTS = 329,
     K_MINFEATURE = 330,
     K_MUSTJOIN = 331,
     K_NAMESCASESENSITIVE = 332,
     K_NANOSECONDS = 333,
     K_NETS = 334,
     K_NEW = 335,
     K_NONDEFAULTRULE = 336,
     K_NONINVERT = 337,
     K_NONUNATE = 338,
     K_OBS = 339,
     K_OHMS = 340,
     K_OFFSET = 341,
     K_ORIENTATION = 342,
     K_ORIGIN = 343,
     K_OUTPUT = 344,
     K_OUTPUTNOISEMARGIN = 345,
     K_OVERHANG = 346,
     K_OVERLAP = 347,
     K_OFF = 348,
     K_ON = 349,
     K_OVERLAPS = 350,
     K_PAD = 351,
     K_PATH = 352,
     K_PATTERN = 353,
     K_PICOFARADS = 354,
     K_PIN = 355,
     K_PITCH = 356,
     K_PLACED = 357,
     K_POLYGON = 358,
     K_PORT = 359,
     K_POST = 360,
     K_POWER = 361,
     K_PRE = 362,
     K_PULLDOWNRES = 363,
     K_RECT = 364,
     K_RESISTANCE = 365,
     K_RESISTIVE = 366,
     K_RING = 367,
     K_RISE = 368,
     K_RISECS = 369,
     K_RISERS = 370,
     K_RISESATCUR = 371,
     K_RISETHRESH = 372,
     K_RISESATT1 = 373,
     K_RISET0 = 374,
     K_RISEVOLTAGETHRESHOLD = 375,
     K_FALLVOLTAGETHRESHOLD = 376,
     K_ROUTING = 377,
     K_ROWMAJOR = 378,
     K_RPERSQ = 379,
     K_SAMENET = 380,
     K_SCANUSE = 381,
     K_SHAPE = 382,
     K_SHRINKAGE = 383,
     K_SIGNAL = 384,
     K_SITE = 385,
     K_SIZE = 386,
     K_SOURCE = 387,
     K_SPACER = 388,
     K_SPACING = 389,
     K_SPECIALNETS = 390,
     K_STACK = 391,
     K_START = 392,
     K_STEP = 393,
     K_STOP = 394,
     K_STRUCTURE = 395,
     K_SYMMETRY = 396,
     K_TABLE = 397,
     K_THICKNESS = 398,
     K_TIEHIGH = 399,
     K_TIELOW = 400,
     K_TIEOFFR = 401,
     K_TIME = 402,
     K_TIMING = 403,
     K_TO = 404,
     K_TOPIN = 405,
     K_TOPLEFT = 406,
     K_TOPRIGHT = 407,
     K_TOPOFSTACKONLY = 408,
     K_TRISTATE = 409,
     K_TYPE = 410,
     K_UNATENESS = 411,
     K_UNITS = 412,
     K_USE = 413,
     K_VARIABLE = 414,
     K_VERTICAL = 415,
     K_VHI = 416,
     K_VIA = 417,
     K_VIARULE = 418,
     K_VLO = 419,
     K_VOLTAGE = 420,
     K_VOLTS = 421,
     K_WIDTH = 422,
     K_X = 423,
     K_Y = 424,
     T_STRING = 425,
     QSTRING = 426,
     NUMBER = 427,
     K_N = 428,
     K_S = 429,
     K_E = 430,
     K_W = 431,
     K_FN = 432,
     K_FS = 433,
     K_FE = 434,
     K_FW = 435,
     K_R0 = 436,
     K_R90 = 437,
     K_R180 = 438,
     K_R270 = 439,
     K_MX = 440,
     K_MY = 441,
     K_MXR90 = 442,
     K_MYR90 = 443,
     K_USER = 444,
     K_MASTERSLICE = 445,
     K_ENDMACRO = 446,
     K_ENDMACROPIN = 447,
     K_ENDVIARULE = 448,
     K_ENDVIA = 449,
     K_ENDLAYER = 450,
     K_ENDSITE = 451,
     K_CANPLACE = 452,
     K_CANNOTOCCUPY = 453,
     K_TRACKS = 454,
     K_FLOORPLAN = 455,
     K_GCELLGRID = 456,
     K_DEFAULTCAP = 457,
     K_MINPINS = 458,
     K_WIRECAP = 459,
     K_STABLE = 460,
     K_SETUP = 461,
     K_HOLD = 462,
     K_DEFINE = 463,
     K_DEFINES = 464,
     K_DEFINEB = 465,
     K_IF = 466,
     K_THEN = 467,
     K_ELSE = 468,
     K_FALSE = 469,
     K_TRUE = 470,
     K_EQ = 471,
     K_NE = 472,
     K_LE = 473,
     K_LT = 474,
     K_GE = 475,
     K_GT = 476,
     K_OR = 477,
     K_AND = 478,
     K_NOT = 479,
     K_DELAY = 480,
     K_TABLEDIMENSION = 481,
     K_TABLEAXIS = 482,
     K_TABLEENTRIES = 483,
     K_TRANSITIONTIME = 484,
     K_EXTENSION = 485,
     K_PROPDEF = 486,
     K_STRING = 487,
     K_INTEGER = 488,
     K_REAL = 489,
     K_RANGE = 490,
     K_PROPERTY = 491,
     K_VIRTUAL = 492,
     K_BUSBITCHARS = 493,
     K_VERSION = 494,
     K_BEGINEXT = 495,
     K_ENDEXT = 496,
     K_UNIVERSALNOISEMARGIN = 497,
     K_EDGERATETHRESHOLD1 = 498,
     K_CORRECTIONTABLE = 499,
     K_EDGERATESCALEFACTOR = 500,
     K_EDGERATETHRESHOLD2 = 501,
     K_VICTIMNOISE = 502,
     K_NOISETABLE = 503,
     K_EDGERATE = 504,
     K_OUTPUTRESISTANCE = 505,
     K_VICTIMLENGTH = 506,
     K_CORRECTIONFACTOR = 507,
     K_OUTPUTPINANTENNASIZE = 508,
     K_INPUTPINANTENNASIZE = 509,
     K_INOUTPINANTENNASIZE = 510,
     K_CURRENTDEN = 511,
     K_PWL = 512,
     K_ANTENNALENGTHFACTOR = 513,
     K_TAPERRULE = 514,
     K_DIVIDERCHAR = 515,
     K_ANTENNASIZE = 516,
     K_ANTENNAMETALLENGTH = 517,
     K_ANTENNAMETALAREA = 518,
     K_RISESLEWLIMIT = 519,
     K_FALLSLEWLIMIT = 520,
     K_FUNCTION = 521,
     K_BUFFER = 522,
     K_INVERTER = 523,
     K_NAMEMAPSTRING = 524,
     K_NOWIREEXTENSIONATPIN = 525,
     K_WIREEXTENSION = 526,
     K_MESSAGE = 527,
     K_CREATEFILE = 528,
     K_OPENFILE = 529,
     K_CLOSEFILE = 530,
     K_WARNING = 531,
     K_ERROR = 532,
     K_FATALERROR = 533,
     K_RECOVERY = 534,
     K_SKEW = 535,
     K_ANYEDGE = 536,
     K_POSEDGE = 537,
     K_NEGEDGE = 538,
     K_SDFCONDSTART = 539,
     K_SDFCONDEND = 540,
     K_SDFCOND = 541,
     K_MPWH = 542,
     K_MPWL = 543,
     K_PERIOD = 544,
     K_ACCURRENTDENSITY = 545,
     K_DCCURRENTDENSITY = 546,
     K_AVERAGE = 547,
     K_PEAK = 548,
     K_RMS = 549,
     K_FREQUENCY = 550,
     K_CUTAREA = 551,
     K_MEGAHERTZ = 552,
     K_USELENGTHTHRESHOLD = 553,
     K_LENGTHTHRESHOLD = 554,
     K_ANTENNAINPUTGATEAREA = 555,
     K_ANTENNAINOUTDIFFAREA = 556,
     K_ANTENNAOUTPUTDIFFAREA = 557,
     K_ANTENNAAREARATIO = 558,
     K_ANTENNADIFFAREARATIO = 559,
     K_ANTENNACUMAREARATIO = 560,
     K_ANTENNACUMDIFFAREARATIO = 561,
     K_ANTENNAAREAFACTOR = 562,
     K_ANTENNASIDEAREARATIO = 563,
     K_ANTENNADIFFSIDEAREARATIO = 564,
     K_ANTENNACUMSIDEAREARATIO = 565,
     K_ANTENNACUMDIFFSIDEAREARATIO = 566,
     K_ANTENNASIDEAREAFACTOR = 567,
     K_DIFFUSEONLY = 568,
     K_MANUFACTURINGGRID = 569,
     K_ANTENNACELL = 570,
     K_CLEARANCEMEASURE = 571,
     K_EUCLIDEAN = 572,
     K_MAXXY = 573,
     K_USEMINSPACING = 574,
     K_ROWMINSPACING = 575,
     K_ROWABUTSPACING = 576,
     K_FLIP = 577,
     K_NONE = 578,
     K_ANTENNAPARTIALMETALAREA = 579,
     K_ANTENNAPARTIALMETALSIDEAREA = 580,
     K_ANTENNAGATEAREA = 581,
     K_ANTENNADIFFAREA = 582,
     K_ANTENNAMAXAREACAR = 583,
     K_ANTENNAMAXSIDEAREACAR = 584,
     K_ANTENNAPARTIALCUTAREA = 585,
     K_ANTENNAMAXCUTCAR = 586,
     K_SLOTWIREWIDTH = 587,
     K_SLOTWIRELENGTH = 588,
     K_SLOTWIDTH = 589,
     K_SLOTLENGTH = 590,
     K_MAXADJACENTSLOTSPACING = 591,
     K_MAXCOAXIALSLOTSPACING = 592,
     K_MAXEDGESLOTSPACING = 593,
     K_SPLITWIREWIDTH = 594,
     K_MINIMUMDENSITY = 595,
     K_MAXIMUMDENSITY = 596,
     K_DENSITYCHECKWINDOW = 597,
     K_DENSITYCHECKSTEP = 598,
     K_FILLACTIVESPACING = 599,
     K_MINIMUMCUT = 600,
     K_ADJACENTCUTS = 601,
     K_ANTENNAMODEL = 602,
     K_BUMP = 603,
     K_ENCLOSURE = 604,
     K_FROMABOVE = 605,
     K_FROMBELOW = 606,
     K_IMPLANT = 607,
     K_LENGTH = 608,
     K_MAXVIASTACK = 609,
     K_AREAIO = 610,
     K_BLACKBOX = 611,
     K_MAXWIDTH = 612,
     K_MINENCLOSEDAREA = 613,
     K_MINSTEP = 614,
     K_ORIENT = 615,
     K_OXIDE1 = 616,
     K_OXIDE2 = 617,
     K_OXIDE3 = 618,
     K_OXIDE4 = 619,
     K_PARALLELRUNLENGTH = 620,
     K_MINWIDTH = 621,
     K_PROTRUSIONWIDTH = 622,
     K_SPACINGTABLE = 623,
     K_WITHIN = 624,
     K_ABOVE = 625,
     K_BELOW = 626,
     K_CENTERTOCENTER = 627,
     K_CUTSIZE = 628,
     K_CUTSPACING = 629,
     K_DENSITY = 630,
     K_DIAG45 = 631,
     K_DIAG135 = 632,
     K_DIAGMINEDGELENGTH = 633,
     K_DIAGSPACING = 634,
     K_DIAGPITCH = 635,
     K_DIAGWIDTH = 636,
     K_GENERATED = 637,
     K_GROUNDSENSITIVITY = 638,
     K_HARDSPACING = 639,
     K_INSIDECORNER = 640,
     K_LAYERS = 641,
     K_LENGTHSUM = 642,
     K_MICRONS = 643,
     K_MINCUTS = 644,
     K_MINSIZE = 645,
     K_NETEXPR = 646,
     K_OUTSIDECORNER = 647,
     K_PREFERENCLOSURE = 648,
     K_ROWCOL = 649,
     K_ROWPATTERN = 650,
     K_SOFT = 651,
     K_SUPPLYSENSITIVITY = 652,
     K_USEVIA = 653,
     K_USEVIARULE = 654,
     K_WELLTAP = 655,
     K_ARRAYCUTS = 656,
     K_ARRAYSPACING = 657,
     K_ANTENNAAREADIFFREDUCEPWL = 658,
     K_ANTENNAAREAMINUSDIFF = 659,
     K_ANTENNACUMROUTINGPLUSCUT = 660,
     K_ANTENNAGATEPLUSDIFF = 661,
     K_ENDOFLINE = 662,
     K_ENDOFNOTCHWIDTH = 663,
     K_EXCEPTEXTRACUT = 664,
     K_EXCEPTSAMEPGNET = 665,
     K_EXCEPTPGNET = 666,
     K_LONGARRAY = 667,
     K_MAXEDGES = 668,
     K_NOTCHLENGTH = 669,
     K_NOTCHSPACING = 670,
     K_ORTHOGONAL = 671,
     K_PARALLELEDGE = 672,
     K_PARALLELOVERLAP = 673,
     K_PGONLY = 674,
     K_PRL = 675,
     K_TWOEDGES = 676,
     K_TWOWIDTHS = 677,
     IF = 678,
     LNOT = 679,
     UMINUS = 680
   };
#endif
/* Tokens.  */
#define K_HISTORY 258
#define K_ABUT 259
#define K_ABUTMENT 260
#define K_ACTIVE 261
#define K_ANALOG 262
#define K_ARRAY 263
#define K_AREA 264
#define K_BLOCK 265
#define K_BOTTOMLEFT 266
#define K_BOTTOMRIGHT 267
#define K_BY 268
#define K_CAPACITANCE 269
#define K_CAPMULTIPLIER 270
#define K_CLASS 271
#define K_CLOCK 272
#define K_CLOCKTYPE 273
#define K_COLUMNMAJOR 274
#define K_DESIGNRULEWIDTH 275
#define K_INFLUENCE 276
#define K_CORE 277
#define K_CORNER 278
#define K_COVER 279
#define K_CPERSQDIST 280
#define K_CURRENT 281
#define K_CURRENTSOURCE 282
#define K_CUT 283
#define K_DEFAULT 284
#define K_DATABASE 285
#define K_DATA 286
#define K_DIELECTRIC 287
#define K_DIRECTION 288
#define K_DO 289
#define K_EDGECAPACITANCE 290
#define K_EEQ 291
#define K_END 292
#define K_ENDCAP 293
#define K_FALL 294
#define K_FALLCS 295
#define K_FALLT0 296
#define K_FALLSATT1 297
#define K_FALLRS 298
#define K_FALLSATCUR 299
#define K_FALLTHRESH 300
#define K_FEEDTHRU 301
#define K_FIXED 302
#define K_FOREIGN 303
#define K_FROMPIN 304
#define K_GENERATE 305
#define K_GENERATOR 306
#define K_GROUND 307
#define K_HEIGHT 308
#define K_HORIZONTAL 309
#define K_INOUT 310
#define K_INPUT 311
#define K_INPUTNOISEMARGIN 312
#define K_COMPONENTPIN 313
#define K_INTRINSIC 314
#define K_INVERT 315
#define K_IRDROP 316
#define K_ITERATE 317
#define K_IV_TABLES 318
#define K_LAYER 319
#define K_LEAKAGE 320
#define K_LEQ 321
#define K_LIBRARY 322
#define K_MACRO 323
#define K_MATCH 324
#define K_MAXDELAY 325
#define K_MAXLOAD 326
#define K_METALOVERHANG 327
#define K_MILLIAMPS 328
#define K_MILLIWATTS 329
#define K_MINFEATURE 330
#define K_MUSTJOIN 331
#define K_NAMESCASESENSITIVE 332
#define K_NANOSECONDS 333
#define K_NETS 334
#define K_NEW 335
#define K_NONDEFAULTRULE 336
#define K_NONINVERT 337
#define K_NONUNATE 338
#define K_OBS 339
#define K_OHMS 340
#define K_OFFSET 341
#define K_ORIENTATION 342
#define K_ORIGIN 343
#define K_OUTPUT 344
#define K_OUTPUTNOISEMARGIN 345
#define K_OVERHANG 346
#define K_OVERLAP 347
#define K_OFF 348
#define K_ON 349
#define K_OVERLAPS 350
#define K_PAD 351
#define K_PATH 352
#define K_PATTERN 353
#define K_PICOFARADS 354
#define K_PIN 355
#define K_PITCH 356
#define K_PLACED 357
#define K_POLYGON 358
#define K_PORT 359
#define K_POST 360
#define K_POWER 361
#define K_PRE 362
#define K_PULLDOWNRES 363
#define K_RECT 364
#define K_RESISTANCE 365
#define K_RESISTIVE 366
#define K_RING 367
#define K_RISE 368
#define K_RISECS 369
#define K_RISERS 370
#define K_RISESATCUR 371
#define K_RISETHRESH 372
#define K_RISESATT1 373
#define K_RISET0 374
#define K_RISEVOLTAGETHRESHOLD 375
#define K_FALLVOLTAGETHRESHOLD 376
#define K_ROUTING 377
#define K_ROWMAJOR 378
#define K_RPERSQ 379
#define K_SAMENET 380
#define K_SCANUSE 381
#define K_SHAPE 382
#define K_SHRINKAGE 383
#define K_SIGNAL 384
#define K_SITE 385
#define K_SIZE 386
#define K_SOURCE 387
#define K_SPACER 388
#define K_SPACING 389
#define K_SPECIALNETS 390
#define K_STACK 391
#define K_START 392
#define K_STEP 393
#define K_STOP 394
#define K_STRUCTURE 395
#define K_SYMMETRY 396
#define K_TABLE 397
#define K_THICKNESS 398
#define K_TIEHIGH 399
#define K_TIELOW 400
#define K_TIEOFFR 401
#define K_TIME 402
#define K_TIMING 403
#define K_TO 404
#define K_TOPIN 405
#define K_TOPLEFT 406
#define K_TOPRIGHT 407
#define K_TOPOFSTACKONLY 408
#define K_TRISTATE 409
#define K_TYPE 410
#define K_UNATENESS 411
#define K_UNITS 412
#define K_USE 413
#define K_VARIABLE 414
#define K_VERTICAL 415
#define K_VHI 416
#define K_VIA 417
#define K_VIARULE 418
#define K_VLO 419
#define K_VOLTAGE 420
#define K_VOLTS 421
#define K_WIDTH 422
#define K_X 423
#define K_Y 424
#define T_STRING 425
#define QSTRING 426
#define NUMBER 427
#define K_N 428
#define K_S 429
#define K_E 430
#define K_W 431
#define K_FN 432
#define K_FS 433
#define K_FE 434
#define K_FW 435
#define K_R0 436
#define K_R90 437
#define K_R180 438
#define K_R270 439
#define K_MX 440
#define K_MY 441
#define K_MXR90 442
#define K_MYR90 443
#define K_USER 444
#define K_MASTERSLICE 445
#define K_ENDMACRO 446
#define K_ENDMACROPIN 447
#define K_ENDVIARULE 448
#define K_ENDVIA 449
#define K_ENDLAYER 450
#define K_ENDSITE 451
#define K_CANPLACE 452
#define K_CANNOTOCCUPY 453
#define K_TRACKS 454
#define K_FLOORPLAN 455
#define K_GCELLGRID 456
#define K_DEFAULTCAP 457
#define K_MINPINS 458
#define K_WIRECAP 459
#define K_STABLE 460
#define K_SETUP 461
#define K_HOLD 462
#define K_DEFINE 463
#define K_DEFINES 464
#define K_DEFINEB 465
#define K_IF 466
#define K_THEN 467
#define K_ELSE 468
#define K_FALSE 469
#define K_TRUE 470
#define K_EQ 471
#define K_NE 472
#define K_LE 473
#define K_LT 474
#define K_GE 475
#define K_GT 476
#define K_OR 477
#define K_AND 478
#define K_NOT 479
#define K_DELAY 480
#define K_TABLEDIMENSION 481
#define K_TABLEAXIS 482
#define K_TABLEENTRIES 483
#define K_TRANSITIONTIME 484
#define K_EXTENSION 485
#define K_PROPDEF 486
#define K_STRING 487
#define K_INTEGER 488
#define K_REAL 489
#define K_RANGE 490
#define K_PROPERTY 491
#define K_VIRTUAL 492
#define K_BUSBITCHARS 493
#define K_VERSION 494
#define K_BEGINEXT 495
#define K_ENDEXT 496
#define K_UNIVERSALNOISEMARGIN 497
#define K_EDGERATETHRESHOLD1 498
#define K_CORRECTIONTABLE 499
#define K_EDGERATESCALEFACTOR 500
#define K_EDGERATETHRESHOLD2 501
#define K_VICTIMNOISE 502
#define K_NOISETABLE 503
#define K_EDGERATE 504
#define K_OUTPUTRESISTANCE 505
#define K_VICTIMLENGTH 506
#define K_CORRECTIONFACTOR 507
#define K_OUTPUTPINANTENNASIZE 508
#define K_INPUTPINANTENNASIZE 509
#define K_INOUTPINANTENNASIZE 510
#define K_CURRENTDEN 511
#define K_PWL 512
#define K_ANTENNALENGTHFACTOR 513
#define K_TAPERRULE 514
#define K_DIVIDERCHAR 515
#define K_ANTENNASIZE 516
#define K_ANTENNAMETALLENGTH 517
#define K_ANTENNAMETALAREA 518
#define K_RISESLEWLIMIT 519
#define K_FALLSLEWLIMIT 520
#define K_FUNCTION 521
#define K_BUFFER 522
#define K_INVERTER 523
#define K_NAMEMAPSTRING 524
#define K_NOWIREEXTENSIONATPIN 525
#define K_WIREEXTENSION 526
#define K_MESSAGE 527
#define K_CREATEFILE 528
#define K_OPENFILE 529
#define K_CLOSEFILE 530
#define K_WARNING 531
#define K_ERROR 532
#define K_FATALERROR 533
#define K_RECOVERY 534
#define K_SKEW 535
#define K_ANYEDGE 536
#define K_POSEDGE 537
#define K_NEGEDGE 538
#define K_SDFCONDSTART 539
#define K_SDFCONDEND 540
#define K_SDFCOND 541
#define K_MPWH 542
#define K_MPWL 543
#define K_PERIOD 544
#define K_ACCURRENTDENSITY 545
#define K_DCCURRENTDENSITY 546
#define K_AVERAGE 547
#define K_PEAK 548
#define K_RMS 549
#define K_FREQUENCY 550
#define K_CUTAREA 551
#define K_MEGAHERTZ 552
#define K_USELENGTHTHRESHOLD 553
#define K_LENGTHTHRESHOLD 554
#define K_ANTENNAINPUTGATEAREA 555
#define K_ANTENNAINOUTDIFFAREA 556
#define K_ANTENNAOUTPUTDIFFAREA 557
#define K_ANTENNAAREARATIO 558
#define K_ANTENNADIFFAREARATIO 559
#define K_ANTENNACUMAREARATIO 560
#define K_ANTENNACUMDIFFAREARATIO 561
#define K_ANTENNAAREAFACTOR 562
#define K_ANTENNASIDEAREARATIO 563
#define K_ANTENNADIFFSIDEAREARATIO 564
#define K_ANTENNACUMSIDEAREARATIO 565
#define K_ANTENNACUMDIFFSIDEAREARATIO 566
#define K_ANTENNASIDEAREAFACTOR 567
#define K_DIFFUSEONLY 568
#define K_MANUFACTURINGGRID 569
#define K_ANTENNACELL 570
#define K_CLEARANCEMEASURE 571
#define K_EUCLIDEAN 572
#define K_MAXXY 573
#define K_USEMINSPACING 574
#define K_ROWMINSPACING 575
#define K_ROWABUTSPACING 576
#define K_FLIP 577
#define K_NONE 578
#define K_ANTENNAPARTIALMETALAREA 579
#define K_ANTENNAPARTIALMETALSIDEAREA 580
#define K_ANTENNAGATEAREA 581
#define K_ANTENNADIFFAREA 582
#define K_ANTENNAMAXAREACAR 583
#define K_ANTENNAMAXSIDEAREACAR 584
#define K_ANTENNAPARTIALCUTAREA 585
#define K_ANTENNAMAXCUTCAR 586
#define K_SLOTWIREWIDTH 587
#define K_SLOTWIRELENGTH 588
#define K_SLOTWIDTH 589
#define K_SLOTLENGTH 590
#define K_MAXADJACENTSLOTSPACING 591
#define K_MAXCOAXIALSLOTSPACING 592
#define K_MAXEDGESLOTSPACING 593
#define K_SPLITWIREWIDTH 594
#define K_MINIMUMDENSITY 595
#define K_MAXIMUMDENSITY 596
#define K_DENSITYCHECKWINDOW 597
#define K_DENSITYCHECKSTEP 598
#define K_FILLACTIVESPACING 599
#define K_MINIMUMCUT 600
#define K_ADJACENTCUTS 601
#define K_ANTENNAMODEL 602
#define K_BUMP 603
#define K_ENCLOSURE 604
#define K_FROMABOVE 605
#define K_FROMBELOW 606
#define K_IMPLANT 607
#define K_LENGTH 608
#define K_MAXVIASTACK 609
#define K_AREAIO 610
#define K_BLACKBOX 611
#define K_MAXWIDTH 612
#define K_MINENCLOSEDAREA 613
#define K_MINSTEP 614
#define K_ORIENT 615
#define K_OXIDE1 616
#define K_OXIDE2 617
#define K_OXIDE3 618
#define K_OXIDE4 619
#define K_PARALLELRUNLENGTH 620
#define K_MINWIDTH 621
#define K_PROTRUSIONWIDTH 622
#define K_SPACINGTABLE 623
#define K_WITHIN 624
#define K_ABOVE 625
#define K_BELOW 626
#define K_CENTERTOCENTER 627
#define K_CUTSIZE 628
#define K_CUTSPACING 629
#define K_DENSITY 630
#define K_DIAG45 631
#define K_DIAG135 632
#define K_DIAGMINEDGELENGTH 633
#define K_DIAGSPACING 634
#define K_DIAGPITCH 635
#define K_DIAGWIDTH 636
#define K_GENERATED 637
#define K_GROUNDSENSITIVITY 638
#define K_HARDSPACING 639
#define K_INSIDECORNER 640
#define K_LAYERS 641
#define K_LENGTHSUM 642
#define K_MICRONS 643
#define K_MINCUTS 644
#define K_MINSIZE 645
#define K_NETEXPR 646
#define K_OUTSIDECORNER 647
#define K_PREFERENCLOSURE 648
#define K_ROWCOL 649
#define K_ROWPATTERN 650
#define K_SOFT 651
#define K_SUPPLYSENSITIVITY 652
#define K_USEVIA 653
#define K_USEVIARULE 654
#define K_WELLTAP 655
#define K_ARRAYCUTS 656
#define K_ARRAYSPACING 657
#define K_ANTENNAAREADIFFREDUCEPWL 658
#define K_ANTENNAAREAMINUSDIFF 659
#define K_ANTENNACUMROUTINGPLUSCUT 660
#define K_ANTENNAGATEPLUSDIFF 661
#define K_ENDOFLINE 662
#define K_ENDOFNOTCHWIDTH 663
#define K_EXCEPTEXTRACUT 664
#define K_EXCEPTSAMEPGNET 665
#define K_EXCEPTPGNET 666
#define K_LONGARRAY 667
#define K_MAXEDGES 668
#define K_NOTCHLENGTH 669
#define K_NOTCHSPACING 670
#define K_ORTHOGONAL 671
#define K_PARALLELEDGE 672
#define K_PARALLELOVERLAP 673
#define K_PGONLY 674
#define K_PRL 675
#define K_TWOEDGES 676
#define K_TWOWIDTHS 677
#define IF 678
#define LNOT 679
#define UMINUS 680




/* Copy the first part of user declarations.  */
#line 34 "lef.y"

 #include <string.h>
 #include <stdlib.h>
 #include <math.h>
 /* # include <sys/types.h> */

 /* # include <sys/stat.h> */

 /* # include <fcntl.h> */

 /* # include <unistd.h> */

 /* # ifndef	WIN32 */

 /* # include <sys/param.h> */

 /* # endif */

 #include "FEF_stringhash.h"
 #include "lex.h"
 #include "lefiDefs.h"
 #include "lefiUser.h"
 #include "lefiUtil.h"
 #include "lefrCallBacks.h"
 #ifdef WIN32
 #include <malloc.h>
 #endif
  
 #define LYPROP_ECAP "EDGE_CAPACITANCE"
 
 #define YYINITDEPTH 10000  /* pcr 640902 - initialize the yystacksize to 300 */
 /* this may need to increase in a design gets */

 /* larger and a polygon has around 300 sizes */

 /* 11/21/2003 - incrreased to 500, design from */

 /* Artisan is greater than 300, need to find a */

 /* way to dynamically increase the size */

 /* 2/10/2004 - increased to 1000 for pcr 686073 */

 /* 3/22/2004 - increased to 2000 for pcr 695879 */

 /* 9/29/2004 - double the size for pcr 746865 */

 /* tried to overwrite the yyoverflow definition */

 /* it is impossible due to the union structure */

 /* 10/03/2006 - increased to 10000 for pcr 913695 */

 
 #define YYMAXDEPTH 300000  /* 1/24/2008 - increased from 150000 */
 /* This value has to be greater than YYINITDEPTH */

 int lefRetVal;
 char *outMsg;
 
 static char lefPropDefType; /* save the current type of the property */

 
 /* Macro to describe how we handle a callback.
 * If the function was set then call it.
 * If the function returns non zero then there was an error
 * so call the error routine and exit.
 */

 #define CALLBACK(func, typ, data) \
 if (!lef_errors) { \
 if (func) { \
 if ((lefRetVal = (*func)(typ, data, lefrUserData)) == 0) { \
 } else { \
 	 return lefRetVal; \
 } \
 } \
 }
 
 #define CHKERR() \
 if (lef_errors > 20) { \
 lefError(1020, "Too many syntax errors."); \
 lef_errors = 0; \
 return 1; \
 }
 
 extern int lefNlines, lefNtokens;
 extern FILE* lefFile;
 
 /* *********************************************************************/
/* *********************************************************************/

 #define C_EQ 0
 #define C_NE 1
 #define C_LT 2
 #define C_LE 3
 #define C_GT 4
 #define C_GE 5
 
 extern int lefDumbMode; /* for communicating with parser
 */
extern int lefNoNum; /* likewise, says no numbers for the next N tokens
 */
extern int lefRealNum; /* Next token will be a real number
 */
extern int lefNlToken; /* likewise
  */
extern int lefDefIf; /* likewise
  */
extern int lefNdRule; /* for communicating with parser
 */
extern int lefInPropDef; /* for tracking if inside propertydefinitions
 */
extern int lefInProp; /* for tracking if inside property
 */

 extern int lef_errors; /* from lex.cpph
 */
extern char* Hist_text; /* for BEGINEXT - extension
 */

 extern int doneLib; /* keep track if the file is done parsing
 */
extern int ge56almostDone; /* lef file has Extension section
 */
extern int ge56done; /* done reading 5.6 and beyond library
 */

 extern char* lefrFileName;
 
 static int ignoreVersion = 0; /* ignore checking version number
 */

 int lefNamesCaseSensitive = TRUE; /* always true in 5.6
 */
int lefReaderCaseSensitive = FALSE; /* default to 0
 */

 int lefrShiftCase = 0; /* if user wants to shift to all uppercase
 */

 int lefrRelaxMode = FALSE; /* relax mode?
 */

 /* XXXXXXXXXXXXX check out these variables */

 double lef_save_x, lef_save_y; /* for interpreting (*) notation of LEF/DEF
 */

 /* #define STRING_LIST_SIZE 1024 */
 /* char string_list[STRING_LIST_SIZE]; */

 
 static char temp_name[258];
 static char* layerName;
 static char* viaName;
 static char* viaRuleName;
 static char* nonDefaultRuleName;
 static char* siteName;
 static char* arrayName;
 static char* macroName;
 static char* pinName;
 
 static int siteDef, symDef, sizeDef, pinDef, obsDef, origDef;
 static int useLenThr;
 static int layerCut, layerMastOver, layerRout, layerDir;
 static lefiAntennaEnum antennaType = lefiAntennaAR; /* 5.4 - antenna type */

 static int arrayCutsVal; /* keep track the arraycuts value */

 static int arrayCutsWar; /* keep track if warning has already printed */

 static int viaRuleLayer; /* keep track number of layer in a viarule */

 static int viaLayer; /* keep track number of layer in a via */

 static int ndLayer; /* keep track number of layer in a nondefault */

 static int numVia; /* keep track number of via */

 static int viaRuleHasDir; /* viarule layer has direction construct */

 static int viaRuleHasEnc; /* viarule layer has enclosure construct */

 static int ndRule = 0; /* keep track if inside nondefaultrule */

 static int ndLayerWidth; /* keep track if width is set at ndLayer */

 static int ndLayerSpace; /* keep track if spacing is set at ndLayer */

 static int isGenerate; /* keep track if viarule has generate */

 static int hasType; /* keep track of type in layer */

 static int hasPitch; /* keep track of pitch in layer */

 static int hasWidth; /* keep track of width in layer */

 static int hasDirection; /* keep track of direction in layer */

 static int hasParallel; /* keep track of parallelrunlength */

 static int hasInfluence; /* keep track of influence */

 static int hasTwoWidths; /* keep track of twoWidths */

 static int hasLayerMincut; /* keep track of layer minimumcut */

 static int hasManufactur = 0; /* keep track of manufacture is after unit */

 static int hasMinfeature = 0; /* keep track of minfeature is after unit */

 static int hasPRP; /* keep track if path, rect or poly is def */

 static int needGeometry; /* keep track if path/rect/poly is defined */

 static int hasViaRule_layer = 0; /* keep track at least viarule or layer */

 static int hasSamenet; /* keep track if samenet is defined in spacing */

 static int hasSite; /* keep track if SITE has defined for pre 5.6 */

 static int hasSiteClass = 0; /* keep track if SITE has CLASS */

 static int hasSiteSize = 0; /* keep track if SITE has SIZE */

 static int hasSpCenter = 0; /* keep track if LAYER SPACING has CENTER */

 static int hasSpSamenet = 0; /* keep track if LAYER SPACING has SAMENET */

 static int hasSpParallel = 0; /* keep track if LAYER SPACING has PARALLEL */

 static int hasSpLayer = 0; /* keep track if LAYER SPACING has LAYER */

 static int hasGeoLayer = 0; /* keep track if Geometries has LAYER */

 
 
 /* the following variables to keep track the number of warnings printed.
 */
static int antennaInoutWarnings = 0;
 static int antennaInputWarnings = 0;
 static int antennaOutputWarnings = 0;
 static int arrayWarnings = 0;
 static int caseSensitiveWarnings = 0;
 static int correctionTableWarnings = 0;
 static int dielectricWarnings = 0;
 static int edgeRateThreshold1Warnings = 0;
 static int edgeRateThreshold2Warnings = 0;
 static int edgeRateScaleFactorWarnings = 0;
 static int inoutAntennaWarnings = 0;
 static int inputAntennaWarnings = 0;
 static int iRDropWarnings = 0;
 static int layerWarnings = 0;
 static int macroWarnings = 0;
 static int maxStackViaWarnings = 0;
 static int minFeatureWarnings = 0;
 static int noiseMarginWarnings = 0;
 static int noiseTableWarnings = 0;
 static int nonDefaultWarnings = 0;
 static int noWireExtensionWarnings = 0;
 static int outputAntennaWarnings = 0;
 static int pinWarnings = 0;
 static int siteWarnings = 0;
 static int spacingWarnings = 0;
 static int timingWarnings = 0;
 static int unitsWarnings = 0;
 static int useMinSpacingWarnings = 0;
 static int viaRuleWarnings = 0;
 static int viaWarnings = 0;
 static double layerCutSpacing = 0;
 
 int spParallelLength; /* the number of layer parallelrunlength */

 
 lefiNum macroNum; /* for origin & size callback */

 
 int comp_str(char *s1, int op, char *s2)
 {
 int k = strcmp(s1, s2);
 switch (op) {
 	case C_EQ: return k == 0;
 	case C_NE: return k != 0;
 	case C_GT: return k > 0;
 	case C_GE: return k >= 0;
 	case C_LT: return k < 0;
 	case C_LE: return k <= 0;
 	}
 return 0;
 }
 int comp_num(double s1, int op, double s2)
 {
 double k = s1 - s2;
 switch (op) {
 	case C_EQ: return k == 0;
 	case C_NE: return k != 0;
 	case C_GT: return k > 0;
 	case C_GE: return k >= 0;
 	case C_LT: return k < 0;
 	case C_LE: return k <= 0;
 	}
 return 0;
 }
 /* 
double convert_name2num(char *versionName)
 {
 char majorNm[80];
 char minorNm[80];
 char *subMinorNm = NULL;
 char *p1;
 char *versionNm = strdup(versionName);
 
 double major = 0, minor = 0, subMinor = 0;
 double version;
 
 sscanf(versionNm, "%[^.].%s", majorNm, minorNm);
 if (p1 = strchr(minorNm, '.')) {
 subMinorNm = p1+1;
 *p1 = '\0';
 }
 major = atof(majorNm);
 minor = atof(minorNm);
 if (subMinorNm)
 subMinor = atof(subMinorNm);
 
 version = major;
 
 if (minor > 0)
 version = major + minor/10;
 
 if (subMinor > 0)
 version = version + subMinor/1000;
 
 lefFree(versionNm);
 return version;
 }
 */

 
 int validNum(int values) {
 switch (values) {
 case 100:
 case 200:
 case 1000:
 case 2000:
 return 1;
 case 10000:
 case 20000:
 if (versionNum < 5.6) {
 if (lefrUnitsCbk) {
 if (unitsWarnings++ < lefrUnitsWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "Error found when processing LEF file '%s'\nUnit %d is a version 5.6 or later syntax\nYour lef file is defined with version %g",
 lefrFileName, values, versionNum);
 lefError(1501, outMsg);
 lefFree(outMsg);
 }
 }
 return 0;
 } else
 return 1;
 }
 if (unitsWarnings++ < lefrUnitsWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "The value %d defined for LEF UNITS DATABASE MICRONS is invalid\n. Correct value is 100, 200, 1000, 2000, 10000, or 20000", values);
 lefError(1502, outMsg);
 lefFree(outMsg);
 }
 CHKERR();
 return 0;
 }
 
 int zeroOrGt(double values) {
 if (values < 0)
 return 0;
 return 1;
 }
 
 void resetVars() {
 hasVer = 0;
 hasNameCase = 0;
 hasBusBit = 0;
 hasDivChar = 0;
 hasManufactur = 0;
 hasMinfeature = 0;
 hasSite = 0;
 antennaInoutWarnings = 0;
 antennaInputWarnings = 0;
 antennaOutputWarnings = 0;
 arrayWarnings = 0;
 caseSensitiveWarnings = 0;
 correctionTableWarnings = 0;
 dielectricWarnings = 0;
 edgeRateThreshold1Warnings = 0;
 edgeRateThreshold2Warnings = 0;
 edgeRateScaleFactorWarnings = 0;
 inoutAntennaWarnings = 0;
 inputAntennaWarnings = 0;
 iRDropWarnings = 0;
 layerWarnings = 0;
 macroWarnings = 0;
 maxStackViaWarnings = 0;
 minFeatureWarnings = 0;
 noiseMarginWarnings = 0;
 noiseTableWarnings = 0;
 nonDefaultWarnings = 0;
 noWireExtensionWarnings = 0;
 outputAntennaWarnings = 0;
 siteWarnings = 0;
 spacingWarnings = 0;
 timingWarnings = 0;
 unitsWarnings = 0;
 useMinSpacingWarnings = 0;
 viaRuleWarnings = 0;
 viaWarnings = 0;
 return;
 }
 
 

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 457 "lef.y"
{
 	double dval ;
 	int integer ;
 	char * string ;
 	lefPOINT pt;
 }
/* Line 193 of yacc.c.  */
#line 1384 "lef.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 1397 "lef.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2091

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  437
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  445
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1010
/* YYNRULES -- Number of states.  */
#define YYNSTATES  2029

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   680

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     434,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     431,   432,   427,   426,     2,   425,     2,   428,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   430,
     435,   433,   436,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     429
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,     8,    13,    17,    21,    22,    25,
      27,    28,    31,    33,    35,    37,    39,    41,    43,    45,
      47,    49,    51,    53,    55,    57,    59,    61,    63,    65,
      67,    69,    71,    73,    75,    77,    79,    81,    83,    85,
      87,    89,    91,    93,    95,    97,    99,   101,   103,   105,
     107,   111,   115,   119,   123,   127,   132,   136,   138,   140,
     142,   144,   146,   148,   153,   155,   156,   159,   164,   169,
     174,   179,   184,   189,   194,   199,   203,   204,   208,   209,
     213,   214,   217,   218,   219,   233,   237,   241,   246,   250,
     255,   259,   264,   268,   272,   276,   280,   281,   288,   289,
     299,   303,   308,   316,   321,   329,   333,   337,   341,   345,
     349,   353,   357,   361,   368,   375,   376,   381,   382,   387,
     392,   397,   398,   399,   409,   410,   411,   421,   425,   426,
     431,   435,   436,   441,   442,   448,   452,   453,   458,   462,
     463,   468,   469,   475,   476,   481,   484,   488,   492,   493,
     494,   504,   508,   512,   516,   520,   524,   528,   532,   536,
     540,   544,   549,   553,   557,   561,   565,   566,   572,   573,
     583,   584,   590,   598,   599,   604,   605,   613,   614,   622,
     626,   630,   631,   637,   638,   640,   641,   644,   645,   648,
     653,   654,   655,   656,   657,   669,   670,   671,   681,   682,
     692,   693,   696,   701,   702,   704,   706,   707,   708,   713,
     716,   717,   720,   721,   724,   725,   728,   729,   731,   733,
     734,   739,   740,   743,   745,   748,   751,   753,   755,   757,
     759,   760,   768,   769,   772,   774,   775,   777,   779,   781,
     783,   784,   785,   786,   799,   800,   801,   807,   808,   814,
     815,   821,   822,   825,   827,   830,   833,   836,   839,   841,
     844,   849,   851,   854,   859,   861,   864,   869,   871,   873,
     875,   877,   879,   881,   883,   885,   887,   889,   890,   893,
     895,   897,   899,   901,   902,   905,   906,   911,   912,   915,
     916,   923,   924,   927,   928,   931,   938,   942,   943,   951,
     952,   957,   959,   962,   966,   970,   971,   972,   973,  1000,
    1001,  1004,  1009,  1014,  1021,  1022,  1027,  1029,  1031,  1034,
    1035,  1038,  1040,  1042,  1046,  1047,  1052,  1054,  1056,  1059,
    1062,  1065,  1068,  1071,  1075,  1080,  1084,  1085,  1089,  1091,
    1093,  1095,  1097,  1099,  1101,  1103,  1105,  1107,  1109,  1111,
    1113,  1115,  1117,  1119,  1121,  1124,  1125,  1130,  1131,  1134,
    1139,  1140,  1148,  1149,  1153,  1154,  1158,  1164,  1165,  1173,
    1174,  1176,  1178,  1181,  1182,  1184,  1186,  1189,  1190,  1195,
    1197,  1200,  1203,  1206,  1209,  1212,  1213,  1216,  1220,  1221,
    1226,  1227,  1230,  1234,  1238,  1243,  1249,  1254,  1260,  1264,
    1268,  1272,  1273,  1277,  1281,  1283,  1286,  1287,  1290,  1296,
    1303,  1305,  1309,  1311,  1314,  1315,  1318,  1322,  1323,  1326,
    1329,  1332,  1337,  1341,  1342,  1343,  1344,  1353,  1355,  1358,
    1359,  1362,  1363,  1366,  1368,  1370,  1372,  1374,  1376,  1378,
    1380,  1384,  1388,  1393,  1394,  1399,  1401,  1404,  1407,  1410,
    1413,  1414,  1415,  1416,  1417,  1430,  1431,  1434,  1438,  1442,
    1447,  1452,  1456,  1460,  1464,  1465,  1469,  1470,  1474,  1475,
    1478,  1484,  1486,  1488,  1490,  1494,  1498,  1502,  1506,  1507,
    1510,  1512,  1514,  1516,  1517,  1522,  1523,  1526,  1527,  1531,
    1534,  1539,  1540,  1545,  1546,  1550,  1551,  1555,  1556,  1559,
    1561,  1563,  1565,  1567,  1569,  1571,  1573,  1575,  1577,  1579,
    1581,  1583,  1585,  1589,  1593,  1595,  1597,  1599,  1601,  1602,
    1607,  1609,  1612,  1616,  1617,  1620,  1622,  1624,  1626,  1629,
    1632,  1635,  1639,  1641,  1644,  1646,  1648,  1651,  1654,  1656,
    1658,  1660,  1662,  1665,  1667,  1669,  1672,  1675,  1677,  1679,
    1681,  1683,  1685,  1687,  1689,  1691,  1693,  1695,  1697,  1699,
    1701,  1703,  1705,  1707,  1709,  1711,  1715,  1720,  1724,  1728,
    1732,  1736,  1740,  1743,  1747,  1752,  1756,  1757,  1762,  1763,
    1768,  1772,  1776,  1778,  1780,  1786,  1790,  1791,  1795,  1796,
    1800,  1801,  1804,  1807,  1811,  1816,  1820,  1825,  1831,  1832,
    1837,  1841,  1843,  1847,  1851,  1855,  1859,  1863,  1867,  1871,
    1875,  1879,  1883,  1887,  1888,  1893,  1894,  1900,  1901,  1907,
    1908,  1914,  1918,  1922,  1926,  1930,  1934,  1938,  1942,  1946,
    1950,  1955,  1959,  1960,  1965,  1970,  1973,  1978,  1983,  1988,
    1992,  1996,  2001,  2006,  2011,  2016,  2021,  2026,  2031,  2036,
    2037,  2042,  2043,  2048,  2049,  2054,  2055,  2060,  2062,  2064,
    2066,  2068,  2070,  2073,  2076,  2079,  2082,  2086,  2090,  2095,
    2099,  2103,  2105,  2106,  2110,  2112,  2114,  2116,  2118,  2120,
    2122,  2124,  2126,  2128,  2130,  2131,  2133,  2135,  2137,  2140,
    2141,  2142,  2150,  2154,  2159,  2166,  2171,  2178,  2185,  2194,
    2196,  2197,  2200,  2201,  2203,  2204,  2207,  2210,  2212,  2214,
    2215,  2218,  2219,  2225,  2226,  2234,  2242,  2254,  2259,  2260,
    2261,  2272,  2273,  2274,  2285,  2292,  2299,  2300,  2303,  2305,
    2312,  2319,  2323,  2326,  2328,  2333,  2334,  2337,  2338,  2339,
    2347,  2348,  2351,  2357,  2358,  2363,  2367,  2369,  2372,  2373,
    2376,  2377,  2382,  2383,  2388,  2389,  2400,  2410,  2414,  2418,
    2423,  2428,  2433,  2438,  2443,  2448,  2453,  2458,  2462,  2470,
    2479,  2486,  2490,  2494,  2498,  2501,  2503,  2505,  2507,  2509,
    2511,  2513,  2515,  2517,  2519,  2521,  2523,  2525,  2527,  2529,
    2531,  2533,  2536,  2542,  2544,  2547,  2548,  2553,  2561,  2563,
    2565,  2567,  2569,  2571,  2573,  2576,  2578,  2581,  2582,  2587,
    2588,  2592,  2593,  2597,  2598,  2601,  2602,  2607,  2608,  2613,
    2614,  2619,  2620,  2625,  2630,  2631,  2636,  2642,  2644,  2647,
    2648,  2651,  2652,  2657,  2658,  2663,  2664,  2667,  2673,  2674,
    2681,  2682,  2689,  2690,  2697,  2698,  2705,  2706,  2713,  2714,
    2716,  2718,  2720,  2723,  2725,  2728,  2732,  2736,  2740,  2744,
    2747,  2751,  2758,  2760,  2764,  2768,  2772,  2776,  2780,  2784,
    2788,  2792,  2796,  2800,  2803,  2807,  2814,  2816,  2818,  2822,
    2826,  2833,  2835,  2837,  2839,  2841,  2843,  2845,  2847,  2849,
    2851,  2853,  2854,  2860,  2861,  2864,  2865,  2871,  2872,  2878,
    2879,  2885,  2886,  2892,  2893,  2899,  2900,  2906,  2907,  2913,
    2914,  2920,  2921,  2926,  2927,  2932,  2934,  2937,  2940,  2941,
    2943,  2946,  2952,  2956,  2957,  2958,  2965,  2966,  2968,  2969,
    2971,  2972,  2976,  2977,  2979,  2980,  2983,  2985,  2986,  2990,
    2992,  2993,  2994,  2995,  3001,  3002,  3009,  3012,  3013,  3019,
    3022,  3028,  3029,  3036,  3039,  3046,  3047,  3049,  3050,  3052,
    3053,  3054,  3058,  3059,  3063,  3068,  3072,  3076,  3080,  3081,
    3089,  3092,  3094,  3097,  3101,  3103,  3104,  3110,  3112,  3115,
    3117,  3120,  3121,  3129,  3131,  3134,  3135,  3143,  3146,  3148,
    3151,  3155,  3157,  3158,  3164,  3166,  3169,  3171,  3174,  3175,
    3183,  3185,  3188,  3192,  3196,  3200,  3204,  3208,  3212,  3213,
    3215
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     438,     0,    -1,   443,   880,   444,    -1,    -1,   239,   440,
     170,   430,    -1,   260,   171,   430,    -1,   238,   171,   430,
      -1,    -1,   443,   445,    -1,     1,    -1,    -1,    37,    67,
      -1,   439,    -1,   442,    -1,   446,    -1,   454,    -1,   458,
      -1,   555,    -1,   587,    -1,   588,    -1,   441,    -1,   447,
      -1,   794,    -1,   606,    -1,   621,    -1,   620,    -1,   612,
      -1,   643,    -1,   660,    -1,   774,    -1,   798,    -1,   622,
      -1,   809,    -1,   846,    -1,   847,    -1,   849,    -1,   848,
      -1,   850,    -1,   862,    -1,   874,    -1,   875,    -1,   876,
      -1,   877,    -1,   878,    -1,   879,    -1,   448,    -1,   449,
      -1,   450,    -1,   553,    -1,   796,    -1,    77,    94,   430,
      -1,    77,    93,   430,    -1,   270,    94,   430,    -1,   270,
      93,   430,    -1,   314,   172,   430,    -1,   319,   452,   453,
     430,    -1,   316,   451,   430,    -1,   318,    -1,   317,    -1,
      84,    -1,   100,    -1,    94,    -1,    93,    -1,   455,   456,
      37,   157,    -1,   157,    -1,    -1,   456,   457,    -1,   147,
      78,   172,   430,    -1,    14,    99,   172,   430,    -1,   110,
      85,   172,   430,    -1,   106,    74,   172,   430,    -1,    26,
      73,   172,   430,    -1,   165,   166,   172,   430,    -1,    30,
     388,   172,   430,    -1,   295,   297,   172,   430,    -1,   459,
     463,   461,    -1,    -1,    64,   460,   170,    -1,    -1,    37,
     462,   170,    -1,    -1,   463,   464,    -1,    -1,    -1,   402,
     465,   491,   492,   374,   172,   401,   172,   134,   172,   466,
     493,   430,    -1,   155,   540,   430,    -1,   101,   172,   430,
      -1,   101,   172,   172,   430,    -1,   380,   172,   430,    -1,
     380,   172,   172,   430,    -1,    86,   172,   430,    -1,    86,
     172,   172,   430,    -1,   381,   172,   430,    -1,   379,   172,
     430,    -1,   167,   172,   430,    -1,     9,   172,   430,    -1,
      -1,   134,   172,   467,   831,   834,   430,    -1,    -1,   368,
     416,   369,   172,   134,   172,   468,   503,   430,    -1,    33,
     541,   430,    -1,   110,   124,   172,   430,    -1,   110,   124,
     257,   431,   538,   432,   430,    -1,    14,    25,   172,   430,
      -1,    14,    25,   257,   431,   536,   432,   430,    -1,    53,
     172,   430,    -1,   271,   172,   430,    -1,   143,   172,   430,
      -1,   128,   172,   430,    -1,    15,   172,   430,    -1,    35,
     172,   430,    -1,   258,   172,   430,    -1,   256,   172,   430,
      -1,   256,   257,   431,   534,   432,   430,    -1,   256,   431,
     172,   172,   432,   430,    -1,    -1,   236,   469,   532,   430,
      -1,    -1,   290,   521,   470,   522,    -1,   290,   521,   172,
     430,    -1,   291,   292,   172,   430,    -1,    -1,    -1,   291,
     292,   296,   172,   471,   531,   430,   472,   529,    -1,    -1,
      -1,   291,   292,   167,   172,   473,   531,   430,   474,   529,
      -1,   303,   172,   430,    -1,    -1,   304,   475,   516,   430,
      -1,   305,   172,   430,    -1,    -1,   306,   476,   516,   430,
      -1,    -1,   307,   172,   477,   520,   430,    -1,   308,   172,
     430,    -1,    -1,   309,   478,   516,   430,    -1,   310,   172,
     430,    -1,    -1,   311,   479,   516,   430,    -1,    -1,   312,
     172,   480,   520,   430,    -1,    -1,   347,   481,   543,   430,
      -1,   405,   430,    -1,   406,   172,   430,    -1,   404,   172,
     430,    -1,    -1,    -1,   403,   431,   659,   659,   482,   518,
     432,   430,   483,    -1,   332,   172,   430,    -1,   333,   172,
     430,    -1,   334,   172,   430,    -1,   335,   172,   430,    -1,
     336,   172,   430,    -1,   337,   172,   430,    -1,   338,   172,
     430,    -1,   339,   172,   430,    -1,   340,   172,   430,    -1,
     341,   172,   430,    -1,   342,   172,   172,   430,    -1,   343,
     172,   430,    -1,   344,   172,   430,    -1,   357,   172,   430,
      -1,   366,   172,   430,    -1,    -1,   358,   172,   484,   542,
     430,    -1,    -1,   345,   172,   167,   172,   485,   510,   511,
     512,   430,    -1,    -1,   359,   172,   486,   513,   430,    -1,
     367,   172,   353,   172,   167,   172,   430,    -1,    -1,   368,
     487,   495,   430,    -1,    -1,   349,   505,   172,   172,   488,
     506,   430,    -1,    -1,   393,   505,   172,   172,   489,   509,
     430,    -1,   110,   172,   430,    -1,   378,   172,   430,    -1,
      -1,   390,   490,   726,   728,   430,    -1,    -1,   412,    -1,
      -1,   167,   172,    -1,    -1,   494,   493,    -1,   401,   172,
     134,   172,    -1,    -1,    -1,    -1,    -1,   365,   172,   496,
     531,   497,   167,   172,   498,   531,   499,   544,    -1,    -1,
      -1,   422,   167,   172,   550,   172,   500,   531,   501,   547,
      -1,    -1,    21,   167,   172,   369,   172,   134,   172,   502,
     551,    -1,    -1,   504,   503,    -1,   369,   172,   134,   172,
      -1,    -1,   370,    -1,   371,    -1,    -1,    -1,   167,   172,
     507,   508,    -1,   353,   172,    -1,    -1,   409,   172,    -1,
      -1,   167,   172,    -1,    -1,   369,   172,    -1,    -1,   350,
      -1,   351,    -1,    -1,   353,   172,   369,   172,    -1,    -1,
     513,   514,    -1,   515,    -1,   387,   172,    -1,   413,   172,
      -1,   385,    -1,   392,    -1,   138,    -1,   172,    -1,    -1,
     257,   431,   659,   659,   517,   518,   432,    -1,    -1,   518,
     519,    -1,   659,    -1,    -1,   313,    -1,   293,    -1,   292,
      -1,   294,    -1,    -1,    -1,    -1,   295,   172,   523,   531,
     430,   524,   526,   228,   172,   525,   531,   430,    -1,    -1,
      -1,   296,   172,   527,   531,   430,    -1,    -1,   167,   172,
     528,   531,   430,    -1,    -1,   228,   172,   530,   531,   430,
      -1,    -1,   531,   172,    -1,   533,    -1,   532,   533,    -1,
     170,   170,    -1,   170,   171,    -1,   170,   172,    -1,   535,
      -1,   534,   535,    -1,   431,   172,   172,   432,    -1,   537,
      -1,   536,   537,    -1,   431,   172,   172,   432,    -1,   539,
      -1,   538,   539,    -1,   431,   172,   172,   432,    -1,   122,
      -1,    28,    -1,    92,    -1,   190,    -1,   237,    -1,   352,
      -1,    54,    -1,   160,    -1,   376,    -1,   377,    -1,    -1,
     167,   172,    -1,   361,    -1,   362,    -1,   363,    -1,   364,
      -1,    -1,   544,   545,    -1,    -1,   167,   172,   546,   531,
      -1,    -1,   548,   547,    -1,    -1,   167,   172,   550,   172,
     549,   531,    -1,    -1,   420,   172,    -1,    -1,   551,   552,
      -1,   167,   172,   369,   172,   134,   172,    -1,   354,   172,
     430,    -1,    -1,   354,   172,   235,   554,   170,   170,   430,
      -1,    -1,   558,   556,   566,   583,    -1,   162,    -1,   557,
     170,    -1,   557,   170,    29,    -1,   557,   170,   382,    -1,
      -1,    -1,    -1,   163,   560,   170,   430,   373,   172,   172,
     430,   386,   561,   170,   170,   170,   430,   374,   172,   172,
     430,   349,   172,   172,   172,   172,   430,   562,   563,    -1,
      -1,   563,   564,    -1,   394,   172,   172,   430,    -1,    88,
     172,   172,   430,    -1,    86,   172,   172,   172,   172,   430,
      -1,    -1,    98,   565,   170,   430,    -1,   559,    -1,   567,
      -1,   569,   568,    -1,    -1,   568,   569,    -1,   573,    -1,
     577,    -1,   110,   172,   430,    -1,    -1,   236,   570,   571,
     430,    -1,   153,    -1,   572,    -1,   571,   572,    -1,   170,
     172,    -1,   170,   171,    -1,   170,   170,    -1,   574,   430,
      -1,   574,   659,   430,    -1,   574,   659,   576,   430,    -1,
     574,   576,   430,    -1,    -1,    48,   575,   170,    -1,   173,
      -1,   176,    -1,   174,    -1,   175,    -1,   177,    -1,   180,
      -1,   178,    -1,   179,    -1,   181,    -1,   182,    -1,   183,
      -1,   184,    -1,   186,    -1,   188,    -1,   185,    -1,   187,
      -1,   578,   580,    -1,    -1,    64,   579,   170,   430,    -1,
      -1,   580,   581,    -1,   109,   659,   659,   430,    -1,    -1,
     103,   582,   726,   727,   727,   728,   430,    -1,    -1,    37,
     584,   170,    -1,    -1,   163,   586,   170,    -1,   585,   591,
     598,   592,   604,    -1,    -1,   585,    50,   590,   589,   591,
     592,   604,    -1,    -1,    29,    -1,   597,    -1,   591,   597,
      -1,    -1,   593,    -1,   594,    -1,   593,   594,    -1,    -1,
     236,   595,   596,   430,    -1,   594,    -1,   596,   594,    -1,
     170,   170,    -1,   170,   171,    -1,   170,   172,    -1,   600,
     602,    -1,    -1,   598,   599,    -1,   557,   170,   430,    -1,
      -1,    64,   601,   170,   430,    -1,    -1,   602,   603,    -1,
      33,    54,   430,    -1,    33,   160,   430,    -1,   349,   172,
     172,   430,    -1,   167,   172,   149,   172,   430,    -1,   109,
     659,   659,   430,    -1,   134,   172,    13,   172,   430,    -1,
     110,   172,   430,    -1,    91,   172,   430,    -1,    72,   172,
     430,    -1,    -1,    37,   605,   170,    -1,   607,   609,   608,
      -1,   134,    -1,    37,   134,    -1,    -1,   609,   610,    -1,
     611,   170,   170,   172,   430,    -1,   611,   170,   170,   172,
     136,   430,    -1,   125,    -1,   613,   615,   614,    -1,    61,
      -1,    37,    61,    -1,    -1,   615,   616,    -1,   619,   617,
     430,    -1,    -1,   617,   618,    -1,   172,   172,    -1,   142,
     170,    -1,    75,   172,   172,   430,    -1,    32,   172,   430,
      -1,    -1,    -1,    -1,    81,   623,   170,   624,   627,   628,
     625,   626,    -1,    37,    -1,    37,   170,    -1,    -1,   384,
     430,    -1,    -1,   628,   629,    -1,   636,    -1,   555,    -1,
     606,    -1,   633,    -1,   630,    -1,   631,    -1,   632,    -1,
     398,   170,   430,    -1,   399,   170,   430,    -1,   389,   170,
     172,   430,    -1,    -1,   236,   634,   635,   430,    -1,   633,
      -1,   635,   633,    -1,   170,   170,    -1,   170,   171,    -1,
     170,   172,    -1,    -1,    -1,    -1,    -1,    64,   637,   170,
     638,   167,   172,   430,   639,   641,    37,   640,   170,    -1,
      -1,   641,   642,    -1,   134,   172,   430,    -1,   271,   172,
     430,    -1,   110,   124,   172,   430,    -1,    14,    25,   172,
     430,    -1,    35,   172,   430,    -1,   381,   172,   430,    -1,
     644,   648,   646,    -1,    -1,   130,   645,   170,    -1,    -1,
      37,   647,   170,    -1,    -1,   648,   649,    -1,   131,   172,
      13,   172,   430,    -1,   651,    -1,   650,    -1,   654,    -1,
      16,    96,   430,    -1,    16,    22,   430,    -1,    16,   237,
     430,    -1,   141,   652,   430,    -1,    -1,   652,   653,    -1,
     168,    -1,   169,    -1,   182,    -1,    -1,   395,   655,   656,
     430,    -1,    -1,   656,   657,    -1,    -1,   170,   576,   658,
      -1,   172,   172,    -1,   431,   172,   172,   432,    -1,    -1,
     662,   666,   661,   664,    -1,    -1,    68,   663,   170,    -1,
      -1,    37,   665,   170,    -1,    -1,   666,   667,    -1,   674,
      -1,   679,    -1,   680,    -1,   681,    -1,   670,    -1,   683,
      -1,   682,    -1,   684,    -1,   685,    -1,   687,    -1,   692,
      -1,   689,    -1,   693,    -1,   266,   267,   430,    -1,   266,
     268,   430,    -1,   742,    -1,   744,    -1,   751,    -1,   753,
      -1,    -1,   236,   668,   669,   430,    -1,   673,    -1,   669,
     673,    -1,   141,   671,   430,    -1,    -1,   671,   672,    -1,
     168,    -1,   169,    -1,   182,    -1,   170,   172,    -1,   170,
     171,    -1,   170,   170,    -1,    16,   675,   430,    -1,    24,
      -1,    24,   348,    -1,   112,    -1,    10,    -1,    10,   356,
      -1,    10,   396,    -1,   323,    -1,   348,    -1,    96,    -1,
     237,    -1,    96,   676,    -1,    22,    -1,    23,    -1,    22,
     677,    -1,    38,   678,    -1,    56,    -1,    89,    -1,    55,
      -1,   106,    -1,   133,    -1,   355,    -1,    46,    -1,   144,
      -1,   145,    -1,   133,    -1,   315,    -1,   400,    -1,   107,
      -1,   105,    -1,   151,    -1,   152,    -1,    11,    -1,    12,
      -1,    51,   170,   430,    -1,    50,   170,   170,   430,    -1,
     132,   189,   430,    -1,   132,    50,   430,    -1,   132,    10,
     430,    -1,   106,   172,   430,    -1,    88,   659,   430,    -1,
     574,   430,    -1,   574,   659,   430,    -1,   574,   659,   576,
     430,    -1,   574,   576,   430,    -1,    -1,    36,   686,   170,
     430,    -1,    -1,    66,   688,   170,   430,    -1,   690,   170,
     430,    -1,   690,   733,   430,    -1,   130,    -1,   130,    -1,
     131,   172,    13,   172,   430,    -1,   694,   698,   696,    -1,
      -1,   100,   695,   170,    -1,    -1,    37,   697,   170,    -1,
      -1,   698,   699,    -1,   574,   430,    -1,   574,   659,   430,
      -1,   574,   659,   576,   430,    -1,   574,   140,   430,    -1,
     574,   140,   659,   430,    -1,   574,   140,   659,   576,   430,
      -1,    -1,    66,   700,   170,   430,    -1,   106,   172,   430,
      -1,   713,    -1,   158,   716,   430,    -1,   126,   717,   430,
      -1,    65,   172,   430,    -1,   117,   172,   430,    -1,    45,
     172,   430,    -1,   116,   172,   430,    -1,    44,   172,   430,
      -1,   164,   172,   430,    -1,   161,   172,   430,    -1,   146,
     172,   430,    -1,   127,   718,   430,    -1,    -1,    76,   701,
     170,   430,    -1,    -1,    90,   702,   172,   172,   430,    -1,
      -1,   250,   703,   172,   172,   430,    -1,    -1,    57,   704,
     172,   172,   430,    -1,    14,   172,   430,    -1,    70,   172,
     430,    -1,    71,   172,   430,    -1,   110,   172,   430,    -1,
     108,   172,   430,    -1,    27,     6,   430,    -1,    27,   111,
     430,    -1,   120,   172,   430,    -1,   121,   172,   430,    -1,
      63,   170,   170,   430,    -1,   259,   170,   430,    -1,    -1,
     236,   705,   711,   430,    -1,   714,   715,   719,    37,    -1,
     714,    37,    -1,   261,   172,   842,   430,    -1,   263,   172,
     842,   430,    -1,   262,   172,   842,   430,    -1,   264,   172,
     430,    -1,   265,   172,   430,    -1,   324,   172,   842,   430,
      -1,   325,   172,   842,   430,    -1,   330,   172,   842,   430,
      -1,   327,   172,   842,   430,    -1,   326,   172,   842,   430,
      -1,   328,   172,   844,   430,    -1,   329,   172,   844,   430,
      -1,   331,   172,   844,   430,    -1,    -1,   347,   706,   710,
     430,    -1,    -1,   391,   707,   171,   430,    -1,    -1,   397,
     708,   170,   430,    -1,    -1,   383,   709,   170,   430,    -1,
     361,    -1,   362,    -1,   363,    -1,   364,    -1,   712,    -1,
     711,   712,    -1,   170,   172,    -1,   170,   171,    -1,   170,
     170,    -1,    33,    56,   430,    -1,    33,    89,   430,    -1,
      33,    89,   154,   430,    -1,    33,    55,   430,    -1,    33,
      46,   430,    -1,   104,    -1,    -1,    16,   675,   430,    -1,
     129,    -1,     7,    -1,   106,    -1,    52,    -1,    17,    -1,
      31,    -1,    56,    -1,    89,    -1,   137,    -1,   139,    -1,
      -1,     5,    -1,   112,    -1,    46,    -1,   720,   723,    -1,
      -1,    -1,    64,   721,   170,   722,   724,   725,   430,    -1,
     167,   172,   430,    -1,    97,   726,   728,   430,    -1,    97,
      62,   726,   728,   732,   430,    -1,   109,   659,   659,   430,
      -1,   109,    62,   659,   659,   732,   430,    -1,   103,   726,
     727,   727,   728,   430,    -1,   103,    62,   726,   727,   727,
     728,   732,   430,    -1,   729,    -1,    -1,   723,   720,    -1,
      -1,   411,    -1,    -1,   134,   172,    -1,    20,   172,    -1,
     659,    -1,   659,    -1,    -1,   728,   727,    -1,    -1,   162,
     659,   730,   170,   430,    -1,    -1,   162,    62,   659,   731,
     170,   732,   430,    -1,    34,   172,    13,   172,   138,   172,
     172,    -1,   170,   172,   172,   576,    34,   172,    13,   172,
     138,   172,   172,    -1,   170,   172,   172,   576,    -1,    -1,
      -1,   168,   172,    34,   172,   138,   172,   735,    64,   736,
     739,    -1,    -1,    -1,   169,   172,    34,   172,   138,   172,
     737,    64,   738,   739,    -1,   168,   172,    34,   172,   138,
     172,    -1,   169,   172,    34,   172,   138,   172,    -1,    -1,
     739,   740,    -1,   170,    -1,   168,   172,    34,   172,   138,
     172,    -1,   169,   172,    34,   172,   138,   172,    -1,   743,
     719,    37,    -1,   743,    37,    -1,    84,    -1,   375,   746,
     745,    37,    -1,    -1,   745,   746,    -1,    -1,    -1,    64,
     747,   170,   430,   748,   750,   749,    -1,    -1,   749,   750,
      -1,   109,   659,   659,   172,   430,    -1,    -1,    18,   752,
     170,   430,    -1,   754,   756,   755,    -1,   148,    -1,    37,
     148,    -1,    -1,   756,   757,    -1,    -1,    49,   758,   772,
     430,    -1,    -1,   150,   759,   773,   430,    -1,    -1,   770,
      59,   172,   172,   760,   769,   159,   172,   172,   430,    -1,
     770,   765,   156,   771,   226,   172,   172,   172,   430,    -1,
     227,   768,   430,    -1,   228,   766,   430,    -1,   115,   172,
     172,   430,    -1,    43,   172,   172,   430,    -1,   114,   172,
     172,   430,    -1,    40,   172,   172,   430,    -1,   118,   172,
     172,   430,    -1,    42,   172,   172,   430,    -1,   119,   172,
     172,   430,    -1,    41,   172,   172,   430,    -1,   156,   771,
     430,    -1,   205,   206,   172,   207,   172,   770,   430,    -1,
     762,   763,   764,   226,   172,   172,   172,   430,    -1,   761,
     226,   172,   172,   172,   430,    -1,   284,   171,   430,    -1,
     285,   171,   430,    -1,   286,   171,   430,    -1,   230,   430,
      -1,   287,    -1,   288,    -1,   289,    -1,   206,    -1,   207,
      -1,   279,    -1,   280,    -1,   281,    -1,   282,    -1,   283,
      -1,   281,    -1,   282,    -1,   283,    -1,   225,    -1,   229,
      -1,   767,    -1,   766,   767,    -1,   431,   172,   172,   172,
     432,    -1,   172,    -1,   768,   172,    -1,    -1,   172,   172,
     172,   172,    -1,   172,   172,   172,   172,   172,   172,   172,
      -1,   113,    -1,    39,    -1,    60,    -1,    82,    -1,    83,
      -1,   170,    -1,   772,   170,    -1,   170,    -1,   773,   170,
      -1,    -1,   776,   780,   775,   778,    -1,    -1,     8,   777,
     170,    -1,    -1,    37,   779,   170,    -1,    -1,   780,   781,
      -1,    -1,   691,   782,   733,   430,    -1,    -1,   197,   783,
     733,   430,    -1,    -1,   198,   784,   733,   430,    -1,    -1,
     199,   785,   734,   430,    -1,   787,   788,    37,   170,    -1,
      -1,   201,   786,   741,   430,    -1,   202,   172,   792,    37,
     202,    -1,   798,    -1,   200,   170,    -1,    -1,   788,   789,
      -1,    -1,   197,   790,   733,   430,    -1,    -1,   198,   791,
     733,   430,    -1,    -1,   792,   793,    -1,   203,   172,   204,
     172,   430,    -1,    -1,   272,   795,   170,   433,   807,   802,
      -1,    -1,   273,   797,   170,   433,   807,   802,    -1,    -1,
     208,   799,   170,   433,   805,   802,    -1,    -1,   209,   800,
     170,   433,   807,   802,    -1,    -1,   210,   801,   170,   433,
     806,   802,    -1,    -1,   430,    -1,   434,    -1,   212,    -1,
     434,   212,    -1,   213,    -1,   434,   213,    -1,   805,   426,
     805,    -1,   805,   425,   805,    -1,   805,   427,   805,    -1,
     805,   428,   805,    -1,   425,   805,    -1,   431,   805,   432,
      -1,   211,   806,   803,   805,   804,   805,    -1,   172,    -1,
     805,   808,   805,    -1,   805,   223,   805,    -1,   805,   222,
     805,    -1,   807,   808,   807,    -1,   807,   223,   807,    -1,
     807,   222,   807,    -1,   806,   216,   806,    -1,   806,   217,
     806,    -1,   806,   223,   806,    -1,   806,   222,   806,    -1,
     224,   806,    -1,   431,   806,   432,    -1,   211,   806,   803,
     806,   804,   806,    -1,   215,    -1,   214,    -1,   807,   426,
     807,    -1,   431,   807,   432,    -1,   211,   806,   803,   807,
     804,   807,    -1,   171,    -1,   218,    -1,   219,    -1,   220,
      -1,   221,    -1,   216,    -1,   217,    -1,   433,    -1,   435,
      -1,   436,    -1,    -1,   231,   810,   811,    37,   231,    -1,
      -1,   811,   812,    -1,    -1,    67,   813,   170,   821,   430,
      -1,    -1,    58,   814,   170,   821,   430,    -1,    -1,   100,
     815,   170,   821,   430,    -1,    -1,    68,   816,   170,   821,
     430,    -1,    -1,   162,   817,   170,   821,   430,    -1,    -1,
     163,   818,   170,   821,   430,    -1,    -1,    64,   819,   170,
     821,   430,    -1,    -1,    81,   820,   170,   821,   430,    -1,
      -1,   233,   822,   829,   830,    -1,    -1,   234,   823,   829,
     830,    -1,   232,    -1,   232,   171,    -1,   269,   170,    -1,
      -1,   298,    -1,    21,   172,    -1,    21,   172,   235,   172,
     172,    -1,   235,   172,   172,    -1,    -1,    -1,   417,   172,
     369,   172,   826,   827,    -1,    -1,   421,    -1,    -1,   419,
      -1,    -1,   235,   172,   172,    -1,    -1,   172,    -1,    -1,
     832,   831,    -1,   372,    -1,    -1,   125,   833,   828,    -1,
     418,    -1,    -1,    -1,    -1,    64,   835,   170,   836,   840,
      -1,    -1,   346,   172,   369,   172,   837,   841,    -1,     9,
     172,    -1,    -1,   235,   172,   172,   838,   824,    -1,   299,
     172,    -1,   299,   172,   235,   172,   172,    -1,    -1,   407,
     172,   369,   172,   839,   825,    -1,   414,   172,    -1,   408,
     172,   415,   172,   414,   172,    -1,    -1,   136,    -1,    -1,
     410,    -1,    -1,    -1,    64,   843,   170,    -1,    -1,    64,
     845,   170,    -1,   242,   172,   172,   430,    -1,   243,   172,
     430,    -1,   246,   172,   430,    -1,   245,   172,   430,    -1,
      -1,   248,   172,   851,   430,   853,   852,   802,    -1,    37,
     248,    -1,   854,    -1,   853,   854,    -1,   249,   172,   430,
      -1,   855,    -1,    -1,   250,   856,   857,   430,   858,    -1,
     172,    -1,   857,   172,    -1,   859,    -1,   858,   859,    -1,
      -1,   251,   172,   430,   860,   247,   861,   430,    -1,   172,
      -1,   861,   172,    -1,    -1,   244,   172,   430,   863,   865,
     864,   802,    -1,    37,   244,    -1,   866,    -1,   865,   866,
      -1,   249,   172,   430,    -1,   867,    -1,    -1,   250,   868,
     869,   430,   870,    -1,   172,    -1,   869,   172,    -1,   871,
      -1,   870,   871,    -1,    -1,   251,   172,   430,   872,   252,
     873,   430,    -1,   172,    -1,   873,   172,    -1,   254,   172,
     430,    -1,   253,   172,   430,    -1,   255,   172,   430,    -1,
     300,   172,   430,    -1,   301,   172,   430,    -1,   302,   172,
     430,    -1,    -1,   881,    -1,   240,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   596,   596,   657,   657,   695,   708,   721,   723,   724,
     729,   735,   767,   767,   767,   767,   768,   768,   768,   768,
     768,   769,   769,   770,   770,   770,   770,   770,   770,   770,
     771,   771,   771,   772,   772,   773,   773,   774,   774,   774,
     775,   775,   776,   776,   776,   776,   777,   777,   777,   778,
     781,   795,   814,   825,   837,   844,   862,   866,   867,   870,
     871,   874,   875,   877,   883,   906,   908,   911,   915,   919,
     923,   927,   931,   935,   944,   949,   956,   956,   991,   991,
    1056,  1057,  1068,  1075,  1066,  1097,  1104,  1110,  1117,  1122,
    1128,  1133,  1139,  1144,  1149,  1156,  1162,  1161,  1208,  1206,
    1230,  1246,  1260,  1272,  1286,  1298,  1312,  1326,  1340,  1354,
    1368,  1397,  1428,  1449,  1468,  1490,  1490,  1497,  1496,  1511,
    1529,  1548,  1575,  1547,  1579,  1606,  1578,  1612,  1657,  1656,
    1700,  1745,  1744,  1789,  1788,  1807,  1852,  1851,  1895,  1940,
    1939,  1984,  1983,  2031,  2029,  2074,  2097,  2121,  2147,  2170,
    2145,  2186,  2216,  2246,  2276,  2306,  2336,  2366,  2396,  2424,
    2446,  2468,  2491,  2513,  2535,  2565,  2597,  2595,  2617,  2615,
    2637,  2635,  2645,  2668,  2666,  2698,  2697,  2722,  2721,  2743,
    2763,  2792,  2791,  2813,  2817,  2823,  2827,  2834,  2838,  2841,
    2861,  2887,  2892,  2899,  2860,  2910,  2915,  2909,  2953,  2952,
    2984,  2988,  2991,  3000,  3002,  3003,  3005,  3008,  3007,  3015,
    3033,  3035,  3053,  3055,  3063,  3065,  3082,  3084,  3105,  3126,
    3128,  3149,  3151,  3159,  3164,  3169,  3185,  3186,  3187,  3190,
    3197,  3196,  3216,  3218,  3227,  3234,  3236,  3274,  3275,  3276,
    3280,  3284,  3288,  3279,  3295,  3298,  3297,  3315,  3314,  3334,
    3333,  3341,  3342,  3348,  3349,  3353,  3365,  3377,  3394,  3396,
    3399,  3406,  3407,  3410,  3417,  3418,  3421,  3428,  3429,  3430,
    3431,  3432,  3433,  3436,  3437,  3438,  3439,  3441,  3442,  3450,
    3456,  3462,  3468,  3477,  3478,  3487,  3486,  3500,  3501,  3520,
    3519,  3534,  3536,  3541,  3542,  3545,  3552,  3593,  3593,  3626,
    3626,  3632,  3636,  3649,  3661,  3674,  3676,  3679,  3674,  3712,
    3714,  3717,  3722,  3728,  3736,  3736,  3743,  3744,  3746,  3749,
    3751,  3755,  3757,  3759,  3763,  3763,  3768,  3780,  3781,  3785,
    3800,  3812,  3826,  3841,  3856,  3871,  3887,  3887,  3891,  3892,
    3893,  3894,  3895,  3896,  3897,  3898,  3899,  3900,  3901,  3902,
    3903,  3904,  3905,  3906,  3908,  3911,  3911,  3919,  3922,  3926,
    3936,  3934,  3959,  3959,  3997,  3997,  4009,  4043,  4042,  4086,
    4088,  4108,  4109,  4112,  4115,  4119,  4120,  4123,  4123,  4130,
    4131,  4135,  4147,  4159,  4175,  4212,  4215,  4218,  4223,  4223,
    4230,  4233,  4237,  4258,  4279,  4323,  4328,  4336,  4341,  4345,
    4383,  4417,  4417,  4438,  4441,  4471,  4485,  4488,  4491,  4506,
    4522,  4527,  4530,  4542,  4551,  4554,  4557,  4565,  4568,  4571,
    4577,  4582,  4599,  4611,  4612,  4624,  4611,  4659,  4664,  4686,
    4689,  4710,  4712,  4716,  4717,  4718,  4719,  4720,  4721,  4722,
    4725,  4744,  4765,  4787,  4787,  4794,  4795,  4799,  4811,  4823,
    4839,  4840,  4851,  4856,  4839,  4902,  4905,  4909,  4915,  4920,
    4949,  4977,  5005,  5027,  5033,  5033,  5045,  5045,  5078,  5081,
    5085,  5095,  5097,  5103,  5107,  5108,  5109,  5111,  5114,  5117,
    5121,  5124,  5127,  5131,  5131,  5135,  5138,  5141,  5141,  5148,
    5150,  5154,  5153,  5161,  5161,  5182,  5182,  5205,  5208,  5217,
    5218,  5219,  5220,  5221,  5222,  5224,  5226,  5228,  5229,  5230,
    5232,  5234,  5236,  5239,  5242,  5244,  5246,  5248,  5250,  5250,
    5257,  5258,  5261,  5275,  5278,  5282,  5285,  5288,  5293,  5308,
    5320,  5333,  5342,  5343,  5363,  5364,  5365,  5385,  5405,  5406,
    5419,  5420,  5421,  5444,  5445,  5453,  5456,  5463,  5464,  5465,
    5466,  5467,  5468,  5471,  5472,  5473,  5474,  5494,  5514,  5536,
    5537,  5538,  5539,  5540,  5541,  5543,  5548,  5555,  5566,  5577,
    5589,  5601,  5668,  5677,  5686,  5695,  5705,  5705,  5710,  5710,
    5723,  5738,  5757,  5761,  5764,  5790,  5797,  5797,  5805,  5805,
    5829,  5830,  5834,  5849,  5864,  5879,  5894,  5909,  5924,  5924,
    5935,  5946,  5950,  5954,  5956,  5967,  5978,  5989,  6000,  6011,
    6022,  6033,  6044,  6048,  6048,  6052,  6052,  6064,  6064,  6076,
    6076,  6088,  6099,  6103,  6107,  6118,  6129,  6140,  6151,  6162,
    6173,  6185,  6189,  6189,  6194,  6211,  6231,  6257,  6283,  6309,
    6313,  6317,  6353,  6389,  6425,  6461,  6497,  6533,  6569,  6606,
    6605,  6639,  6639,  6657,  6657,  6675,  6675,  6695,  6701,  6707,
    6713,  6721,  6722,  6726,  6741,  6753,  6767,  6768,  6769,  6770,
    6771,  6773,  6787,  6789,  6796,  6797,  6798,  6799,  6800,  6801,
    6804,  6805,  6806,  6807,  6811,  6813,  6814,  6815,  6817,  6820,
    6821,  6820,  6841,  6855,  6869,  6883,  6900,  6917,  6932,  6946,
    6949,  6951,  6953,  6955,  6971,  6973,  6988,  7004,  7011,  7018,
    7025,  7029,  7029,  7036,  7036,  7046,  7055,  7073,  7093,  7104,
    7092,  7107,  7118,  7106,  7120,  7132,  7145,  7148,  7151,  7156,
    7168,  7181,  7194,  7215,  7227,  7249,  7251,  7254,  7255,  7254,
    7262,  7264,  7267,  7278,  7278,  7283,  7286,  7290,  7304,  7307,
    7312,  7311,  7321,  7321,  7324,  7323,  7334,  7354,  7356,  7358,
    7363,  7368,  7373,  7378,  7383,  7388,  7393,  7398,  7402,  7408,
    7417,  7424,  7428,  7432,  7436,  7441,  7443,  7445,  7449,  7451,
    7453,  7455,  7459,  7461,  7463,  7467,  7469,  7471,  7475,  7477,
    7481,  7483,  7486,  7494,  7498,  7506,  7507,  7514,  7527,  7529,
    7533,  7535,  7537,  7541,  7545,  7551,  7555,  7561,  7560,  7570,
    7570,  7582,  7582,  7608,  7609,  7613,  7613,  7621,  7621,  7629,
    7629,  7637,  7637,  7644,  7647,  7647,  7654,  7661,  7664,  7672,
    7673,  7677,  7677,  7685,  7685,  7697,  7698,  7701,  7707,  7707,
    7711,  7711,  7715,  7715,  7725,  7725,  7735,  7735,  7748,  7749,
    7750,  7753,  7754,  7758,  7759,  7763,  7764,  7765,  7766,  7767,
    7768,  7769,  7771,  7774,  7775,  7776,  7777,  7778,  7779,  7780,
    7781,  7782,  7783,  7784,  7785,  7786,  7788,  7789,  7792,  7798,
    7800,  7809,  7813,  7814,  7815,  7816,  7817,  7818,  7819,  7820,
    7821,  7825,  7824,  7844,  7845,  7849,  7849,  7863,  7863,  7877,
    7877,  7892,  7892,  7906,  7906,  7920,  7920,  7934,  7934,  7948,
    7948,  7964,  7964,  7969,  7969,  7975,  7980,  7986,  7996,  7997,
    8002,  8012,  8022,  8034,  8036,  8035,  8048,  8049,  8059,  8060,
    8069,  8070,  8079,  8080,  8085,  8088,  8090,  8117,  8115,  8141,
    8166,  8169,  8170,  8169,  8188,  8187,  8214,  8241,  8240,  8248,
    8255,  8267,  8265,  8284,  8300,  8322,  8323,  8333,  8334,  8352,
    8353,  8353,  8359,  8359,  8364,  8379,  8393,  8407,  8422,  8421,
    8429,  8443,  8444,  8448,  8456,  8460,  8459,  8466,  8471,  8478,
    8479,  8483,  8482,  8491,  8496,  8503,  8502,  8511,  8525,  8526,
    8530,  8538,  8542,  8541,  8549,  8554,  8561,  8562,  8567,  8566,
    8575,  8580,  8589,  8617,  8645,  8673,  8713,  8753,  8793,  8795,
    8797
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "K_HISTORY", "K_ABUT", "K_ABUTMENT",
  "K_ACTIVE", "K_ANALOG", "K_ARRAY", "K_AREA", "K_BLOCK", "K_BOTTOMLEFT",
  "K_BOTTOMRIGHT", "K_BY", "K_CAPACITANCE", "K_CAPMULTIPLIER", "K_CLASS",
  "K_CLOCK", "K_CLOCKTYPE", "K_COLUMNMAJOR", "K_DESIGNRULEWIDTH",
  "K_INFLUENCE", "K_CORE", "K_CORNER", "K_COVER", "K_CPERSQDIST",
  "K_CURRENT", "K_CURRENTSOURCE", "K_CUT", "K_DEFAULT", "K_DATABASE",
  "K_DATA", "K_DIELECTRIC", "K_DIRECTION", "K_DO", "K_EDGECAPACITANCE",
  "K_EEQ", "K_END", "K_ENDCAP", "K_FALL", "K_FALLCS", "K_FALLT0",
  "K_FALLSATT1", "K_FALLRS", "K_FALLSATCUR", "K_FALLTHRESH", "K_FEEDTHRU",
  "K_FIXED", "K_FOREIGN", "K_FROMPIN", "K_GENERATE", "K_GENERATOR",
  "K_GROUND", "K_HEIGHT", "K_HORIZONTAL", "K_INOUT", "K_INPUT",
  "K_INPUTNOISEMARGIN", "K_COMPONENTPIN", "K_INTRINSIC", "K_INVERT",
  "K_IRDROP", "K_ITERATE", "K_IV_TABLES", "K_LAYER", "K_LEAKAGE", "K_LEQ",
  "K_LIBRARY", "K_MACRO", "K_MATCH", "K_MAXDELAY", "K_MAXLOAD",
  "K_METALOVERHANG", "K_MILLIAMPS", "K_MILLIWATTS", "K_MINFEATURE",
  "K_MUSTJOIN", "K_NAMESCASESENSITIVE", "K_NANOSECONDS", "K_NETS", "K_NEW",
  "K_NONDEFAULTRULE", "K_NONINVERT", "K_NONUNATE", "K_OBS", "K_OHMS",
  "K_OFFSET", "K_ORIENTATION", "K_ORIGIN", "K_OUTPUT",
  "K_OUTPUTNOISEMARGIN", "K_OVERHANG", "K_OVERLAP", "K_OFF", "K_ON",
  "K_OVERLAPS", "K_PAD", "K_PATH", "K_PATTERN", "K_PICOFARADS", "K_PIN",
  "K_PITCH", "K_PLACED", "K_POLYGON", "K_PORT", "K_POST", "K_POWER",
  "K_PRE", "K_PULLDOWNRES", "K_RECT", "K_RESISTANCE", "K_RESISTIVE",
  "K_RING", "K_RISE", "K_RISECS", "K_RISERS", "K_RISESATCUR",
  "K_RISETHRESH", "K_RISESATT1", "K_RISET0", "K_RISEVOLTAGETHRESHOLD",
  "K_FALLVOLTAGETHRESHOLD", "K_ROUTING", "K_ROWMAJOR", "K_RPERSQ",
  "K_SAMENET", "K_SCANUSE", "K_SHAPE", "K_SHRINKAGE", "K_SIGNAL", "K_SITE",
  "K_SIZE", "K_SOURCE", "K_SPACER", "K_SPACING", "K_SPECIALNETS",
  "K_STACK", "K_START", "K_STEP", "K_STOP", "K_STRUCTURE", "K_SYMMETRY",
  "K_TABLE", "K_THICKNESS", "K_TIEHIGH", "K_TIELOW", "K_TIEOFFR", "K_TIME",
  "K_TIMING", "K_TO", "K_TOPIN", "K_TOPLEFT", "K_TOPRIGHT",
  "K_TOPOFSTACKONLY", "K_TRISTATE", "K_TYPE", "K_UNATENESS", "K_UNITS",
  "K_USE", "K_VARIABLE", "K_VERTICAL", "K_VHI", "K_VIA", "K_VIARULE",
  "K_VLO", "K_VOLTAGE", "K_VOLTS", "K_WIDTH", "K_X", "K_Y", "T_STRING",
  "QSTRING", "NUMBER", "K_N", "K_S", "K_E", "K_W", "K_FN", "K_FS", "K_FE",
  "K_FW", "K_R0", "K_R90", "K_R180", "K_R270", "K_MX", "K_MY", "K_MXR90",
  "K_MYR90", "K_USER", "K_MASTERSLICE", "K_ENDMACRO", "K_ENDMACROPIN",
  "K_ENDVIARULE", "K_ENDVIA", "K_ENDLAYER", "K_ENDSITE", "K_CANPLACE",
  "K_CANNOTOCCUPY", "K_TRACKS", "K_FLOORPLAN", "K_GCELLGRID",
  "K_DEFAULTCAP", "K_MINPINS", "K_WIRECAP", "K_STABLE", "K_SETUP",
  "K_HOLD", "K_DEFINE", "K_DEFINES", "K_DEFINEB", "K_IF", "K_THEN",
  "K_ELSE", "K_FALSE", "K_TRUE", "K_EQ", "K_NE", "K_LE", "K_LT", "K_GE",
  "K_GT", "K_OR", "K_AND", "K_NOT", "K_DELAY", "K_TABLEDIMENSION",
  "K_TABLEAXIS", "K_TABLEENTRIES", "K_TRANSITIONTIME", "K_EXTENSION",
  "K_PROPDEF", "K_STRING", "K_INTEGER", "K_REAL", "K_RANGE", "K_PROPERTY",
  "K_VIRTUAL", "K_BUSBITCHARS", "K_VERSION", "K_BEGINEXT", "K_ENDEXT",
  "K_UNIVERSALNOISEMARGIN", "K_EDGERATETHRESHOLD1", "K_CORRECTIONTABLE",
  "K_EDGERATESCALEFACTOR", "K_EDGERATETHRESHOLD2", "K_VICTIMNOISE",
  "K_NOISETABLE", "K_EDGERATE", "K_OUTPUTRESISTANCE", "K_VICTIMLENGTH",
  "K_CORRECTIONFACTOR", "K_OUTPUTPINANTENNASIZE", "K_INPUTPINANTENNASIZE",
  "K_INOUTPINANTENNASIZE", "K_CURRENTDEN", "K_PWL",
  "K_ANTENNALENGTHFACTOR", "K_TAPERRULE", "K_DIVIDERCHAR", "K_ANTENNASIZE",
  "K_ANTENNAMETALLENGTH", "K_ANTENNAMETALAREA", "K_RISESLEWLIMIT",
  "K_FALLSLEWLIMIT", "K_FUNCTION", "K_BUFFER", "K_INVERTER",
  "K_NAMEMAPSTRING", "K_NOWIREEXTENSIONATPIN", "K_WIREEXTENSION",
  "K_MESSAGE", "K_CREATEFILE", "K_OPENFILE", "K_CLOSEFILE", "K_WARNING",
  "K_ERROR", "K_FATALERROR", "K_RECOVERY", "K_SKEW", "K_ANYEDGE",
  "K_POSEDGE", "K_NEGEDGE", "K_SDFCONDSTART", "K_SDFCONDEND", "K_SDFCOND",
  "K_MPWH", "K_MPWL", "K_PERIOD", "K_ACCURRENTDENSITY",
  "K_DCCURRENTDENSITY", "K_AVERAGE", "K_PEAK", "K_RMS", "K_FREQUENCY",
  "K_CUTAREA", "K_MEGAHERTZ", "K_USELENGTHTHRESHOLD", "K_LENGTHTHRESHOLD",
  "K_ANTENNAINPUTGATEAREA", "K_ANTENNAINOUTDIFFAREA",
  "K_ANTENNAOUTPUTDIFFAREA", "K_ANTENNAAREARATIO",
  "K_ANTENNADIFFAREARATIO", "K_ANTENNACUMAREARATIO",
  "K_ANTENNACUMDIFFAREARATIO", "K_ANTENNAAREAFACTOR",
  "K_ANTENNASIDEAREARATIO", "K_ANTENNADIFFSIDEAREARATIO",
  "K_ANTENNACUMSIDEAREARATIO", "K_ANTENNACUMDIFFSIDEAREARATIO",
  "K_ANTENNASIDEAREAFACTOR", "K_DIFFUSEONLY", "K_MANUFACTURINGGRID",
  "K_ANTENNACELL", "K_CLEARANCEMEASURE", "K_EUCLIDEAN", "K_MAXXY",
  "K_USEMINSPACING", "K_ROWMINSPACING", "K_ROWABUTSPACING", "K_FLIP",
  "K_NONE", "K_ANTENNAPARTIALMETALAREA", "K_ANTENNAPARTIALMETALSIDEAREA",
  "K_ANTENNAGATEAREA", "K_ANTENNADIFFAREA", "K_ANTENNAMAXAREACAR",
  "K_ANTENNAMAXSIDEAREACAR", "K_ANTENNAPARTIALCUTAREA",
  "K_ANTENNAMAXCUTCAR", "K_SLOTWIREWIDTH", "K_SLOTWIRELENGTH",
  "K_SLOTWIDTH", "K_SLOTLENGTH", "K_MAXADJACENTSLOTSPACING",
  "K_MAXCOAXIALSLOTSPACING", "K_MAXEDGESLOTSPACING", "K_SPLITWIREWIDTH",
  "K_MINIMUMDENSITY", "K_MAXIMUMDENSITY", "K_DENSITYCHECKWINDOW",
  "K_DENSITYCHECKSTEP", "K_FILLACTIVESPACING", "K_MINIMUMCUT",
  "K_ADJACENTCUTS", "K_ANTENNAMODEL", "K_BUMP", "K_ENCLOSURE",
  "K_FROMABOVE", "K_FROMBELOW", "K_IMPLANT", "K_LENGTH", "K_MAXVIASTACK",
  "K_AREAIO", "K_BLACKBOX", "K_MAXWIDTH", "K_MINENCLOSEDAREA", "K_MINSTEP",
  "K_ORIENT", "K_OXIDE1", "K_OXIDE2", "K_OXIDE3", "K_OXIDE4",
  "K_PARALLELRUNLENGTH", "K_MINWIDTH", "K_PROTRUSIONWIDTH",
  "K_SPACINGTABLE", "K_WITHIN", "K_ABOVE", "K_BELOW", "K_CENTERTOCENTER",
  "K_CUTSIZE", "K_CUTSPACING", "K_DENSITY", "K_DIAG45", "K_DIAG135",
  "K_DIAGMINEDGELENGTH", "K_DIAGSPACING", "K_DIAGPITCH", "K_DIAGWIDTH",
  "K_GENERATED", "K_GROUNDSENSITIVITY", "K_HARDSPACING", "K_INSIDECORNER",
  "K_LAYERS", "K_LENGTHSUM", "K_MICRONS", "K_MINCUTS", "K_MINSIZE",
  "K_NETEXPR", "K_OUTSIDECORNER", "K_PREFERENCLOSURE", "K_ROWCOL",
  "K_ROWPATTERN", "K_SOFT", "K_SUPPLYSENSITIVITY", "K_USEVIA",
  "K_USEVIARULE", "K_WELLTAP", "K_ARRAYCUTS", "K_ARRAYSPACING",
  "K_ANTENNAAREADIFFREDUCEPWL", "K_ANTENNAAREAMINUSDIFF",
  "K_ANTENNACUMROUTINGPLUSCUT", "K_ANTENNAGATEPLUSDIFF", "K_ENDOFLINE",
  "K_ENDOFNOTCHWIDTH", "K_EXCEPTEXTRACUT", "K_EXCEPTSAMEPGNET",
  "K_EXCEPTPGNET", "K_LONGARRAY", "K_MAXEDGES", "K_NOTCHLENGTH",
  "K_NOTCHSPACING", "K_ORTHOGONAL", "K_PARALLELEDGE", "K_PARALLELOVERLAP",
  "K_PGONLY", "K_PRL", "K_TWOEDGES", "K_TWOWIDTHS", "IF", "LNOT", "'-'",
  "'+'", "'*'", "'/'", "UMINUS", "';'", "'('", "')'", "'='", "'\\n'",
  "'<'", "'>'", "$accept", "lef_file", "version", "@1", "dividerchar",
  "busbitchars", "rules", "end_library", "rule", "case_sensitivity",
  "wireextension", "manufacturing", "useminspacing", "clearancemeasure",
  "clearance_type", "spacing_type", "spacing_value", "units_section",
  "start_units", "units_rules", "units_rule", "layer_rule", "start_layer",
  "@2", "end_layer", "@3", "layer_options", "layer_option", "@4", "@5",
  "@6", "@7", "@8", "@9", "@10", "@11", "@12", "@13", "@14", "@15", "@16",
  "@17", "@18", "@19", "@20", "@21", "@22", "@23", "@24", "@25", "@26",
  "@27", "@28", "@29", "layer_arraySpacing_long",
  "layer_arraySpacing_width", "layer_arraySpacing_arraycuts",
  "layer_arraySpacing_arraycut", "sp_options", "@30", "@31", "@32", "@33",
  "@34", "@35", "@36", "layer_spacingtable_opts", "layer_spacingtable_opt",
  "layer_enclosure_type_opt", "layer_enclosure_width_opt", "@37",
  "layer_enclosure_width_except_opt", "layer_preferenclosure_width_opt",
  "layer_minimumcut_within", "layer_minimumcut_from",
  "layer_minimumcut_length", "layer_minstep_options",
  "layer_minstep_option", "layer_minstep_type", "layer_antenna_pwl", "@38",
  "layer_diffusion_ratios", "layer_diffusion_ratio", "layer_antenna_duo",
  "layer_table_type", "layer_frequency", "@39", "@40", "@41",
  "ac_layer_table_opt", "@42", "@43", "dc_layer_table", "@44",
  "number_list", "layer_prop_list", "layer_prop",
  "current_density_pwl_list", "current_density_pwl", "cap_points",
  "cap_point", "res_points", "res_point", "layer_type", "layer_direction",
  "layer_minen_width", "layer_oxide", "layer_sp_parallel_widths",
  "layer_sp_parallel_width", "@45", "layer_sp_TwoWidths",
  "layer_sp_TwoWidth", "@46", "layer_sp_TwoWidthsPRL",
  "layer_sp_influence_widths", "layer_sp_influence_width", "maxstack_via",
  "@47", "via", "@48", "via_keyword", "start_via", "via_viarule", "@49",
  "@50", "@51", "via_viarule_options", "via_viarule_option", "@52",
  "via_option", "via_other_options", "via_more_options",
  "via_other_option", "@53", "via_prop_list", "via_name_value_pair",
  "via_foreign", "start_foreign", "@54", "orientation", "via_layer_rule",
  "via_layer", "@55", "via_geometries", "via_geometry", "@56", "end_via",
  "@57", "viarule_keyword", "@58", "viarule", "viarule_generate", "@59",
  "viarule_generate_default", "viarule_layer_list", "opt_viarule_props",
  "viarule_props", "viarule_prop", "@60", "viarule_prop_list",
  "viarule_layer", "via_names", "via_name", "viarule_layer_name", "@61",
  "viarule_layer_options", "viarule_layer_option", "end_viarule", "@62",
  "spacing_rule", "start_spacing", "end_spacing", "spacings", "spacing",
  "samenet_keyword", "irdrop", "start_irdrop", "end_irdrop", "ir_tables",
  "ir_table", "ir_table_values", "ir_table_value", "ir_tablename",
  "minfeature", "dielectric", "nondefault_rule", "@63", "@64", "@65",
  "end_nd_rule", "nd_hardspacing", "nd_rules", "nd_rule", "usevia",
  "useviarule", "mincuts", "nd_prop", "@66", "nd_prop_list", "nd_layer",
  "@67", "@68", "@69", "@70", "nd_layer_stmts", "nd_layer_stmt", "site",
  "start_site", "@71", "end_site", "@72", "site_options", "site_option",
  "site_class", "site_symmetry_statement", "site_symmetries",
  "site_symmetry", "site_rowpattern_statement", "@73", "site_rowpatterns",
  "site_rowpattern", "@74", "pt", "macro", "@75", "start_macro", "@76",
  "end_macro", "@77", "macro_options", "macro_option", "@78",
  "macro_prop_list", "macro_symmetry_statement", "macro_symmetries",
  "macro_symmetry", "macro_name_value_pair", "macro_class", "class_type",
  "pad_type", "core_type", "endcap_type", "macro_generator",
  "macro_generate", "macro_source", "macro_power", "macro_origin",
  "macro_foreign", "macro_eeq", "@79", "macro_leq", "@80", "macro_site",
  "macro_site_word", "site_word", "macro_size", "macro_pin",
  "start_macro_pin", "@81", "end_macro_pin", "@82", "macro_pin_options",
  "macro_pin_option", "@83", "@84", "@85", "@86", "@87", "@88", "@89",
  "@90", "@91", "@92", "pin_layer_oxide", "pin_prop_list",
  "pin_name_value_pair", "electrical_direction", "start_macro_port",
  "macro_port_class_option", "macro_pin_use", "macro_scan_use",
  "pin_shape", "geometries", "geometry", "@93", "@94", "geometry_options",
  "layer_exceptpgnet", "layer_spacing", "firstPt", "nextPt", "otherPts",
  "via_placement", "@95", "@96", "stepPattern", "sitePattern",
  "trackPattern", "@97", "@98", "@99", "@100", "trackLayers", "layer_name",
  "gcellPattern", "macro_obs", "start_macro_obs", "macro_density",
  "density_layers", "density_layer", "@101", "@102", "density_layer_rects",
  "density_layer_rect", "macro_clocktype", "@103", "timing",
  "start_timing", "end_timing", "timing_options", "timing_option", "@104",
  "@105", "@106", "one_pin_trigger", "two_pin_trigger", "from_pin_trigger",
  "to_pin_trigger", "delay_or_transition", "list_of_table_entries",
  "table_entry", "list_of_table_axis_numbers", "slew_spec", "risefall",
  "unateness", "list_of_from_strings", "list_of_to_strings", "array",
  "@107", "start_array", "@108", "end_array", "@109", "array_rules",
  "array_rule", "@110", "@111", "@112", "@113", "@114", "floorplan_start",
  "floorplan_list", "floorplan_element", "@115", "@116", "cap_list",
  "one_cap", "msg_statement", "@117", "create_file_statement", "@118",
  "def_statement", "@119", "@120", "@121", "dtrm", "then", "else",
  "expression", "b_expr", "s_expr", "relop", "prop_def_section", "@122",
  "prop_stmts", "prop_stmt", "@123", "@124", "@125", "@126", "@127",
  "@128", "@129", "@130", "prop_define", "@131", "@132",
  "opt_range_second", "opt_endofline", "@133", "opt_endofline_twoedges",
  "opt_samenetPGonly", "opt_def_range", "opt_def_value",
  "layer_spacing_opts", "layer_spacing_opt", "@134",
  "layer_spacing_cut_routing", "@135", "@136", "@137", "@138", "@139",
  "spacing_cut_layer_opt", "opt_adjacentcuts_exceptsame", "opt_layer_name",
  "@140", "req_layer_name", "@141", "universalnoisemargin",
  "edgeratethreshold1", "edgeratethreshold2", "edgeratescalefactor",
  "noisetable", "@142", "end_noisetable", "noise_table_list",
  "noise_table_entry", "output_resistance_entry", "@143", "num_list",
  "victim_list", "victim", "@144", "vnoiselist", "correctiontable", "@145",
  "end_correctiontable", "correction_table_list", "correction_table_item",
  "output_list", "@146", "numo_list", "corr_victim_list", "corr_victim",
  "@147", "corr_list", "input_antenna", "output_antenna", "inout_antenna",
  "antenna_input", "antenna_inout", "antenna_output", "extension_opt",
  "extension", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   679,    45,    43,    42,    47,   680,
      59,    40,    41,    61,    10,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   437,   438,   440,   439,   441,   442,   443,   443,   443,
     444,   444,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     446,   446,   447,   447,   448,   449,   450,   451,   451,   452,
     452,   453,   453,   454,   455,   456,   456,   457,   457,   457,
     457,   457,   457,   457,   457,   458,   460,   459,   462,   461,
     463,   463,   465,   466,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   467,   464,   468,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   469,   464,   470,   464,   464,
     464,   471,   472,   464,   473,   474,   464,   464,   475,   464,
     464,   476,   464,   477,   464,   464,   478,   464,   464,   479,
     464,   480,   464,   481,   464,   464,   464,   464,   482,   483,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   484,   464,   485,   464,
     486,   464,   464,   487,   464,   488,   464,   489,   464,   464,
     464,   490,   464,   491,   491,   492,   492,   493,   493,   494,
     496,   497,   498,   499,   495,   500,   501,   495,   502,   495,
     503,   503,   504,   505,   505,   505,   506,   507,   506,   506,
     508,   508,   509,   509,   510,   510,   511,   511,   511,   512,
     512,   513,   513,   514,   514,   514,   515,   515,   515,   516,
     517,   516,   518,   518,   519,   520,   520,   521,   521,   521,
     523,   524,   525,   522,   526,   527,   526,   528,   526,   530,
     529,   531,   531,   532,   532,   533,   533,   533,   534,   534,
     535,   536,   536,   537,   538,   538,   539,   540,   540,   540,
     540,   540,   540,   541,   541,   541,   541,   542,   542,   543,
     543,   543,   543,   544,   544,   546,   545,   547,   547,   549,
     548,   550,   550,   551,   551,   552,   553,   554,   553,   556,
     555,   557,   558,   558,   558,   560,   561,   562,   559,   563,
     563,   564,   564,   564,   565,   564,   566,   566,   567,   568,
     568,   569,   569,   569,   570,   569,   569,   571,   571,   572,
     572,   572,   573,   573,   573,   573,   575,   574,   576,   576,
     576,   576,   576,   576,   576,   576,   576,   576,   576,   576,
     576,   576,   576,   576,   577,   579,   578,   580,   580,   581,
     582,   581,   584,   583,   586,   585,   587,   589,   588,   590,
     590,   591,   591,   592,   592,   593,   593,   595,   594,   596,
     596,   594,   594,   594,   597,   598,   598,   599,   601,   600,
     602,   602,   603,   603,   603,   603,   603,   603,   603,   603,
     603,   605,   604,   606,   607,   608,   609,   609,   610,   610,
     611,   612,   613,   614,   615,   615,   616,   617,   617,   618,
     619,   620,   621,   623,   624,   625,   622,   626,   626,   627,
     627,   628,   628,   629,   629,   629,   629,   629,   629,   629,
     630,   631,   632,   634,   633,   635,   635,   633,   633,   633,
     637,   638,   639,   640,   636,   641,   641,   642,   642,   642,
     642,   642,   642,   643,   645,   644,   647,   646,   648,   648,
     649,   649,   649,   649,   650,   650,   650,   651,   652,   652,
     653,   653,   653,   655,   654,   656,   656,   658,   657,   659,
     659,   661,   660,   663,   662,   665,   664,   666,   666,   667,
     667,   667,   667,   667,   667,   667,   667,   667,   667,   667,
     667,   667,   667,   667,   667,   667,   667,   667,   668,   667,
     669,   669,   670,   671,   671,   672,   672,   672,   673,   673,
     673,   674,   675,   675,   675,   675,   675,   675,   675,   675,
     675,   675,   675,   675,   675,   675,   675,   676,   676,   676,
     676,   676,   676,   677,   677,   677,   677,   677,   677,   678,
     678,   678,   678,   678,   678,   679,   680,   681,   681,   681,
     682,   683,   684,   684,   684,   684,   686,   685,   688,   687,
     689,   689,   690,   691,   692,   693,   695,   694,   697,   696,
     698,   698,   699,   699,   699,   699,   699,   699,   700,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   701,   699,   702,   699,   703,   699,   704,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   705,   699,   699,   699,   699,   699,   699,   699,
     699,   699,   699,   699,   699,   699,   699,   699,   699,   706,
     699,   707,   699,   708,   699,   709,   699,   710,   710,   710,
     710,   711,   711,   712,   712,   712,   713,   713,   713,   713,
     713,   714,   715,   715,   716,   716,   716,   716,   716,   716,
     717,   717,   717,   717,   718,   718,   718,   718,   719,   721,
     722,   720,   720,   720,   720,   720,   720,   720,   720,   720,
     723,   723,   724,   724,   725,   725,   725,   726,   727,   728,
     728,   730,   729,   731,   729,   732,   733,   733,   735,   736,
     734,   737,   738,   734,   734,   734,   739,   739,   740,   741,
     741,   742,   742,   743,   744,   745,   745,   747,   748,   746,
     749,   749,   750,   752,   751,   753,   754,   755,   756,   756,
     758,   757,   759,   757,   760,   757,   757,   757,   757,   757,
     757,   757,   757,   757,   757,   757,   757,   757,   757,   757,
     757,   757,   757,   757,   757,   761,   761,   761,   762,   762,
     762,   762,   763,   763,   763,   764,   764,   764,   765,   765,
     766,   766,   767,   768,   768,   769,   769,   769,   770,   770,
     771,   771,   771,   772,   772,   773,   773,   775,   774,   777,
     776,   779,   778,   780,   780,   782,   781,   783,   781,   784,
     781,   785,   781,   781,   786,   781,   781,   781,   787,   788,
     788,   790,   789,   791,   789,   792,   792,   793,   795,   794,
     797,   796,   799,   798,   800,   798,   801,   798,   802,   802,
     802,   803,   803,   804,   804,   805,   805,   805,   805,   805,
     805,   805,   805,   806,   806,   806,   806,   806,   806,   806,
     806,   806,   806,   806,   806,   806,   806,   806,   807,   807,
     807,   807,   808,   808,   808,   808,   808,   808,   808,   808,
     808,   810,   809,   811,   811,   813,   812,   814,   812,   815,
     812,   816,   812,   817,   812,   818,   812,   819,   812,   820,
     812,   822,   821,   823,   821,   821,   821,   821,   824,   824,
     824,   824,   824,   825,   826,   825,   827,   827,   828,   828,
     829,   829,   830,   830,   831,   831,   832,   833,   832,   832,
     834,   835,   836,   834,   837,   834,   834,   838,   834,   834,
     834,   839,   834,   834,   834,   840,   840,   841,   841,   842,
     843,   842,   845,   844,   846,   847,   848,   849,   851,   850,
     852,   853,   853,   854,   854,   856,   855,   857,   857,   858,
     858,   860,   859,   861,   861,   863,   862,   864,   865,   865,
     866,   866,   868,   867,   869,   869,   870,   870,   872,   871,
     873,   873,   874,   875,   876,   877,   878,   879,   880,   880,
     881
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     0,     4,     3,     3,     0,     2,     1,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     4,     3,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     2,     4,     4,     4,
       4,     4,     4,     4,     4,     3,     0,     3,     0,     3,
       0,     2,     0,     0,    13,     3,     3,     4,     3,     4,
       3,     4,     3,     3,     3,     3,     0,     6,     0,     9,
       3,     4,     7,     4,     7,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     6,     0,     4,     0,     4,     4,
       4,     0,     0,     9,     0,     0,     9,     3,     0,     4,
       3,     0,     4,     0,     5,     3,     0,     4,     3,     0,
       4,     0,     5,     0,     4,     2,     3,     3,     0,     0,
       9,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     3,     3,     3,     3,     0,     5,     0,     9,
       0,     5,     7,     0,     4,     0,     7,     0,     7,     3,
       3,     0,     5,     0,     1,     0,     2,     0,     2,     4,
       0,     0,     0,     0,    11,     0,     0,     9,     0,     9,
       0,     2,     4,     0,     1,     1,     0,     0,     4,     2,
       0,     2,     0,     2,     0,     2,     0,     1,     1,     0,
       4,     0,     2,     1,     2,     2,     1,     1,     1,     1,
       0,     7,     0,     2,     1,     0,     1,     1,     1,     1,
       0,     0,     0,    12,     0,     0,     5,     0,     5,     0,
       5,     0,     2,     1,     2,     2,     2,     2,     1,     2,
       4,     1,     2,     4,     1,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     2,     1,
       1,     1,     1,     0,     2,     0,     4,     0,     2,     0,
       6,     0,     2,     0,     2,     6,     3,     0,     7,     0,
       4,     1,     2,     3,     3,     0,     0,     0,    26,     0,
       2,     4,     4,     6,     0,     4,     1,     1,     2,     0,
       2,     1,     1,     3,     0,     4,     1,     1,     2,     2,
       2,     2,     2,     3,     4,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     4,     0,     2,     4,
       0,     7,     0,     3,     0,     3,     5,     0,     7,     0,
       1,     1,     2,     0,     1,     1,     2,     0,     4,     1,
       2,     2,     2,     2,     2,     0,     2,     3,     0,     4,
       0,     2,     3,     3,     4,     5,     4,     5,     3,     3,
       3,     0,     3,     3,     1,     2,     0,     2,     5,     6,
       1,     3,     1,     2,     0,     2,     3,     0,     2,     2,
       2,     4,     3,     0,     0,     0,     8,     1,     2,     0,
       2,     0,     2,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     4,     0,     4,     1,     2,     2,     2,     2,
       0,     0,     0,     0,    12,     0,     2,     3,     3,     4,
       4,     3,     3,     3,     0,     3,     0,     3,     0,     2,
       5,     1,     1,     1,     3,     3,     3,     3,     0,     2,
       1,     1,     1,     0,     4,     0,     2,     0,     3,     2,
       4,     0,     4,     0,     3,     0,     3,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     1,     1,     1,     0,     4,
       1,     2,     3,     0,     2,     1,     1,     1,     2,     2,
       2,     3,     1,     2,     1,     1,     2,     2,     1,     1,
       1,     1,     2,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     3,     3,
       3,     3,     2,     3,     4,     3,     0,     4,     0,     4,
       3,     3,     1,     1,     5,     3,     0,     3,     0,     3,
       0,     2,     2,     3,     4,     3,     4,     5,     0,     4,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     0,     4,     0,     5,     0,     5,     0,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     0,     4,     4,     2,     4,     4,     4,     3,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     3,     4,     3,
       3,     1,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     1,     2,     0,
       0,     7,     3,     4,     6,     4,     6,     6,     8,     1,
       0,     2,     0,     1,     0,     2,     2,     1,     1,     0,
       2,     0,     5,     0,     7,     7,    11,     4,     0,     0,
      10,     0,     0,    10,     6,     6,     0,     2,     1,     6,
       6,     3,     2,     1,     4,     0,     2,     0,     0,     7,
       0,     2,     5,     0,     4,     3,     1,     2,     0,     2,
       0,     4,     0,     4,     0,    10,     9,     3,     3,     4,
       4,     4,     4,     4,     4,     4,     4,     3,     7,     8,
       6,     3,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     5,     1,     2,     0,     4,     7,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     0,     4,     0,
       3,     0,     3,     0,     2,     0,     4,     0,     4,     0,
       4,     0,     4,     4,     0,     4,     5,     1,     2,     0,
       2,     0,     4,     0,     4,     0,     2,     5,     0,     6,
       0,     6,     0,     6,     0,     6,     0,     6,     0,     1,
       1,     1,     2,     1,     2,     3,     3,     3,     3,     2,
       3,     6,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     3,     6,     1,     1,     3,     3,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     5,     0,     2,     0,     5,     0,     5,     0,
       5,     0,     5,     0,     5,     0,     5,     0,     5,     0,
       5,     0,     4,     0,     4,     1,     2,     2,     0,     1,
       2,     5,     3,     0,     0,     6,     0,     1,     0,     1,
       0,     3,     0,     1,     0,     2,     1,     0,     3,     1,
       0,     0,     0,     5,     0,     6,     2,     0,     5,     2,
       5,     0,     6,     2,     6,     0,     1,     0,     1,     0,
       0,     3,     0,     3,     4,     3,     3,     3,     0,     7,
       2,     1,     2,     3,     1,     0,     5,     1,     2,     1,
       2,     0,     7,     1,     2,     0,     7,     2,     1,     2,
       3,     1,     0,     5,     1,     2,     1,     2,     0,     7,
       1,     2,     3,     3,     3,     3,     3,     3,     0,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     9,     0,  1008,     1,   809,     0,   412,    76,   493,
       0,     0,   423,   464,   404,    64,   301,   364,   842,   844,
     846,   891,     0,     3,  1010,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   838,   840,     0,     0,
       0,     0,     0,     0,     0,    12,    20,    13,     8,    14,
      21,    45,    46,    47,    15,    65,    16,    80,    48,    17,
       0,   299,     0,    18,    19,    23,   406,    26,   414,    25,
      24,    31,    27,   468,    28,   497,    29,   813,    22,    49,
      30,    32,    33,    34,    36,    35,    37,    38,    39,    40,
      41,    42,    43,    44,    10,  1009,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   893,
       0,     0,     0,     0,     0,     0,     0,   968,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    57,     0,    59,    60,     0,     0,     0,     0,   302,
       0,   369,   388,   385,   371,   390,     0,     0,     0,   491,
     807,     0,     2,   810,   422,    77,   494,     0,    51,    50,
     424,   465,   365,     0,     0,     0,     0,     6,     0,     0,
     965,   985,   967,   966,     0,  1003,  1002,  1004,     5,    53,
      52,     0,     0,  1005,  1006,  1007,    54,    56,    62,    61,
       0,   297,   296,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,     0,
       0,     0,     0,     0,     0,   128,     0,   131,     0,     0,
     136,     0,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,   203,
       0,     0,     0,     0,     0,   173,     0,     0,     0,     0,
     181,   203,    82,     0,     0,     0,     0,    75,    81,   303,
     304,   336,   355,     0,   326,   305,   324,   316,     0,   317,
     319,   321,     0,   322,   357,   370,   367,     0,   372,   373,
     384,     0,   410,   403,   407,     0,     0,     0,   411,   415,
     417,     0,   466,     0,   478,   483,   463,   469,   472,   471,
     473,     0,   743,   576,     0,     0,   578,   733,     0,   586,
       0,   582,     0,     0,   523,   746,   518,     0,     0,     0,
       0,   498,   503,   499,   500,   501,   502,   505,   504,   506,
     507,   508,   510,     0,   509,   511,   590,   514,     0,   515,
     516,   517,   748,   583,   817,   819,   821,     0,   824,     0,
     815,     0,   814,   829,   827,    11,   421,   429,     0,     0,
       0,     0,   897,   907,   895,   901,   909,   899,   903,   905,
     894,     4,   964,     0,     0,     0,     0,    55,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,   273,   274,   275,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,   268,   269,   267,   270,
     271,   272,     0,     0,     0,     0,     0,     0,     0,     0,
     238,   237,   239,   117,     0,     0,     0,     0,     0,   133,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,   205,     0,     0,   166,   170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,   145,
       0,     0,     0,     0,     0,     0,   362,   300,   318,     0,
     338,   340,   341,   339,   342,   344,   345,   343,   346,   347,
     348,   349,   352,   350,   353,   351,   332,     0,     0,     0,
     354,     0,     0,     0,   377,     0,     0,   374,   375,   386,
       0,     0,     0,     0,     0,     0,     0,     0,   391,   405,
       0,   413,   420,     0,     0,     0,     0,     0,     0,     0,
     485,   535,   543,   544,   532,     0,   540,   534,   541,   538,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   737,   735,
     572,     0,     0,   495,   492,     0,     0,     0,   732,   689,
       0,     0,     0,     0,     0,     0,   700,   699,     0,     0,
       0,     0,   828,     0,   835,     0,   811,   808,     0,     0,
     431,   862,     0,     0,     0,   848,   881,     0,     0,   848,
       0,   877,   876,     0,     0,     0,   848,     0,   892,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   992,     0,
     988,   991,     0,   975,     0,   971,   974,   848,   848,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,   109,   100,   110,    79,   105,     0,    90,     0,    86,
       0,     0,   179,   108,   934,   107,    85,    94,     0,     0,
     253,   112,     0,     0,   111,   106,     0,     0,     0,     0,
       0,   127,   229,     0,     0,   130,     0,   235,   135,     0,
     138,     0,   235,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     0,   162,   163,     0,   279,   280,   281,
     282,     0,     0,   164,   277,   221,   165,     0,     0,     0,
       0,     0,     0,   180,    93,     0,    88,    92,   707,   709,
       0,   184,   185,     0,   147,   146,   337,     0,   323,     0,
       0,     0,   327,     0,   320,   489,     0,   335,   333,     0,
     360,     0,   358,   373,   389,   381,   382,   383,     0,     0,
     401,   366,   376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   416,   418,   475,   474,   476,   467,
       0,   480,   481,   482,   477,   479,     0,   536,   537,   553,
     556,   554,   555,   557,   558,   545,   533,   563,   564,   560,
     559,   561,   562,   546,   549,   547,   548,   550,   551,   552,
     542,   531,     0,     0,     0,   565,     0,   571,   587,   570,
       0,   569,   568,   567,   525,   526,   527,   522,   524,     0,
       0,   520,   512,   513,     0,     0,   575,   573,     0,     0,
       0,   580,   581,     0,     0,     0,   588,     0,     0,   619,
       0,     0,   598,     0,     0,   613,   615,   671,     0,     0,
       0,     0,     0,     0,     0,     0,   684,     0,     0,     0,
       0,   632,   617,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   649,   655,   651,
     653,     0,   585,   591,   601,   672,     0,     0,   709,     0,
       0,     0,     0,     0,   711,     0,   731,   688,     0,   799,
       0,     0,     0,     0,   750,   798,     0,     0,     0,     0,
     752,     0,     0,   778,   779,     0,     0,     0,   780,   781,
       0,     0,     0,   775,   776,   777,   745,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   831,   833,   830,   430,   425,     0,
     859,     0,     0,     0,     0,     0,   849,   850,   843,     0,
       0,     0,   845,     0,   873,     0,     0,     0,   886,   887,
     882,   883,   884,   885,     0,     0,   888,   889,   890,     0,
       0,     0,     0,     0,   847,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   848,
     989,     0,     0,     0,   848,   972,   839,   841,     0,    68,
      71,    73,    70,    69,    67,    72,    74,   103,     0,    91,
      87,   101,     0,   937,   936,   939,   940,   934,   255,   256,
     257,   116,   254,     0,     0,   258,     0,   119,     0,   118,
     124,   120,   121,     0,   129,   132,   236,     0,   137,   140,
       0,   161,   168,   144,   175,     0,     0,     0,     0,     0,
       0,   190,     0,   174,    89,     0,   177,     0,     0,   148,
     356,     0,   331,   330,   329,   325,   328,   363,     0,   334,
       0,     0,     0,   379,     0,   387,     0,   392,   393,   400,
     399,     0,   398,     0,     0,     0,     0,   419,     0,     0,
     484,   486,   744,   577,   566,   579,     0,   530,   529,   528,
     519,   521,     0,   734,   736,   574,   496,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   680,   681,   682,   683,     0,   685,   687,
     686,     0,     0,   675,   678,   679,   677,   676,   674,     0,
       0,     0,     0,     0,     0,   959,   959,   959,     0,     0,
     959,   959,   959,   959,     0,     0,   959,     0,     0,     0,
       0,     0,     0,   592,     0,     0,   635,     0,   690,   709,
       0,     0,   708,     0,     0,     0,   713,     0,   692,   701,
     747,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   800,   801,   802,     0,     0,   793,     0,     0,     0,
     790,   774,     0,     0,     0,     0,   782,   783,   784,     0,
       0,   788,   789,     0,   818,   820,     0,     0,   822,     0,
       0,   825,     0,     0,   836,   816,   812,   823,     0,     0,
     450,     0,   443,     0,     0,     0,   434,   435,     0,   432,
     437,   438,   439,   436,   433,   851,     0,     0,   860,   856,
     855,   857,   858,     0,   879,   878,     0,   874,   865,   864,
     863,   869,   870,   872,   871,   868,   867,   866,   915,   911,
     913,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     990,   994,     0,   987,   986,   973,   977,     0,   970,   969,
     298,     0,     0,   261,     0,     0,   264,   928,     0,   941,
       0,     0,     0,     0,     0,     0,     0,   935,     0,     0,
     259,     0,   240,   251,   251,     0,   134,   142,   214,   206,
     278,   167,   228,   226,     0,   227,     0,   171,   222,   223,
       0,     0,     0,   251,   291,   182,   710,   212,   186,     0,
     232,     0,   490,     0,     0,   368,   378,   380,   402,   396,
       0,     0,   394,     0,   408,   470,   487,   584,   738,   717,
     621,   626,   627,   670,   669,   666,     0,   667,   589,   608,
     606,     0,     0,   604,     0,   622,   623,     0,     0,   600,
     625,   624,   607,   605,   628,   629,   603,   612,   611,   602,
     610,   609,     0,     0,   661,     0,   631,   960,     0,     0,
       0,   639,   640,     0,     0,     0,     0,   962,     0,     0,
       0,     0,   657,   658,   659,   660,     0,     0,     0,     0,
     595,     0,   593,     0,     0,     0,   702,     0,   693,     0,
     709,     0,   695,     0,     0,     0,     0,     0,     0,   803,
       0,     0,     0,     0,     0,   805,     0,   767,     0,   794,
     757,     0,   758,   791,   771,   772,   773,     0,   785,   786,
     787,     0,     0,     0,     0,     0,     0,     0,   826,     0,
       0,     0,     0,   447,   448,   449,     0,     0,     0,     0,
     427,   426,   852,     0,     0,     0,     0,     0,   916,   930,
     930,   917,   898,   908,   896,   902,   910,   900,   904,   906,
     995,     0,   978,     0,     0,     0,   262,     0,     0,   265,
     929,   938,   946,     0,     0,   949,     0,     0,     0,   953,
      97,     0,   113,   114,   251,     0,     0,   230,     0,   216,
       0,     0,     0,   224,   225,     0,    98,     0,   191,     0,
       0,     0,     0,     0,     0,     0,     0,   359,   397,   395,
     409,   488,     0,     0,   668,     0,   630,   599,   614,     0,
     665,   664,   663,   633,   662,     0,     0,   636,   638,   637,
     641,   642,   645,   644,     0,   646,   647,   643,   648,   650,
     656,   652,   654,   596,     0,   594,   673,   634,   703,   704,
       0,     0,   709,     0,     0,     0,   712,   762,   766,   764,
     760,   804,   751,   761,   759,   763,   765,   806,   753,     0,
       0,     0,     0,   754,     0,     0,     0,     0,     0,     0,
     832,   834,   451,   445,     0,     0,   440,   441,   428,   853,
       0,     0,     0,     0,     0,   932,   932,     0,   993,   996,
       0,   976,   979,     0,   104,     0,   102,   942,   947,     0,
       0,     0,     0,   260,     0,   252,   125,   122,   232,   215,
     217,   218,   219,   207,   209,   176,   172,   200,     0,     0,
     292,   195,   213,   178,     0,     0,   233,   234,     0,   709,
       0,   740,     0,   620,   616,   618,   961,   963,   597,     0,
       0,     0,     0,   694,     0,   697,   696,     0,     0,     0,
       0,     0,   795,     0,     0,     0,     0,     0,     0,     0,
     444,   446,   442,   854,   861,   880,   875,     0,   933,   912,
     914,     0,   997,     0,   980,   263,   266,   955,   918,     0,
     944,   951,     0,   241,     0,     0,     0,     0,     0,   210,
       0,     0,   200,     0,     0,   251,     0,   149,     0,     0,
       0,   739,     0,   706,   705,   691,     0,     0,   714,     0,
     792,   770,     0,     0,     0,     0,   718,   721,   729,   730,
     837,     0,   931,   998,   981,   956,   943,     0,     0,   919,
     948,   950,   957,   923,     0,   244,     0,   126,   123,   231,
       0,   169,     0,   208,     0,    99,   201,   198,   192,   196,
       0,   150,   306,   361,     0,   741,     0,     0,   698,   768,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   920,
       0,   958,   945,     0,   952,   954,     0,     0,     0,   249,
       0,   211,     0,   293,   251,   287,    83,     0,     0,     0,
       0,   769,     0,     0,     0,   719,   722,   452,     0,     0,
       0,   922,     0,   247,   245,     0,   251,   220,   202,   199,
     193,     0,   197,   287,   187,     0,   742,     0,     0,   796,
       0,   756,   726,   726,   455,  1000,     0,   983,     0,     0,
       0,   251,   251,   242,     0,     0,   294,   283,   291,   288,
       0,     0,   187,     0,   716,   715,     0,   755,   720,   723,
       0,  1001,   999,   984,   982,   921,   924,     0,     0,   251,
     250,     0,   194,     0,     0,    84,   188,     0,     0,   728,
     727,     0,     0,   453,     0,     0,     0,     0,   456,   926,
     248,   246,     0,     0,     0,   284,   289,     0,     0,   797,
       0,     0,     0,     0,     0,     0,     0,   927,   925,   243,
       0,   285,   251,   189,     0,     0,   461,   454,     0,   457,
     458,   462,     0,   251,   290,     0,   460,   459,   295,   286,
       0,     0,     0,     0,     0,     0,     0,   307,   309,   308,
       0,     0,   314,     0,   310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,   315,   311,     0,   313
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    45,   111,    46,    47,     3,   152,    48,    49,
      50,    51,    52,    53,   132,   135,   190,    54,    55,   137,
     202,    56,    57,    98,   267,   407,   138,   268,   476,  1894,
     664,  1697,   424,   677,  1344,  1765,  1343,  1764,   436,   438,
     687,   441,   443,   692,   459,  1370,  1831,   714,  1348,   715,
     469,  1349,  1367,   474,   732,  1078,  1921,  1922,   722,  1363,
    1699,  1864,  1917,  1775,  1865,  1863,  1771,  1772,   462,  1562,
    1769,  1823,  1572,  1559,  1692,  1768,  1067,  1358,  1359,   684,
    1688,  1574,  1706,  1057,   433,  1049,  1554,  1815,  1939,  1858,
    1912,  1911,  1817,  1886,  1555,   669,   670,  1044,  1045,  1322,
    1323,  1325,  1326,   422,   405,  1066,   711,  1942,  1965,  1993,
    1892,  1893,  1982,  1570,  1889,  1916,    58,   388,    59,   140,
      60,    61,   277,   484,  1867,  2008,  2009,  2014,  2017,   278,
     279,   488,   280,   485,   741,   742,   281,   282,   481,   508,
     283,   284,   482,   510,   752,  1090,   487,   743,    62,   105,
      63,    64,   511,   286,   143,   516,   517,   518,   758,  1094,
     144,   289,   519,   145,   287,   290,   528,   761,  1096,    65,
      66,   293,   146,   294,   295,    67,    68,   298,   147,   299,
     533,   775,   300,    69,    70,    71,   103,   367,  1268,  1511,
     600,   958,  1269,  1270,  1271,  1272,  1273,  1506,  1654,  1274,
    1502,  1739,  1904,  1972,  1930,  1958,    72,    73,   104,   306,
     537,   148,   307,   308,   309,   539,   785,   310,   540,   786,
    1111,  1581,  1202,    74,   330,    75,    99,   574,   839,   149,
     331,   565,   830,   332,   564,   828,   831,   333,   551,   810,
     795,   803,   334,   335,   336,   337,   338,   339,   340,   553,
     341,   556,   342,   343,   360,   344,   345,   346,   558,   892,
    1135,   577,   893,  1141,  1144,  1145,  1173,  1138,  1172,  1188,
    1190,  1191,  1189,  1446,  1423,  1424,   894,   895,  1197,  1169,
    1157,  1161,   585,   586,   896,  1456,   907,  1619,  1721,   729,
    1366,  1075,   587,  1207,  1463,  1621,   576,   946,  1844,  1902,
    1845,  1903,  1928,  1950,   949,   347,   348,   349,   835,   569,
     834,  1582,  1781,  1711,   350,   552,   351,   352,   936,   588,
     937,  1215,  1220,  1732,   938,   939,  1239,  1491,  1243,  1229,
    1230,  1227,  1794,   940,  1224,  1470,  1476,    76,   361,    77,
      96,   597,   952,   150,   362,   595,   589,   590,   591,   593,
     363,   598,   956,  1258,  1259,   950,  1254,    78,   124,    79,
     125,    80,   106,   107,   108,   968,  1277,  1661,   615,   616,
     617,   989,    81,   109,   166,   380,   621,   619,   624,   622,
     625,   626,   620,   623,  1302,  1519,  1520,  1810,  1854,  1959,
    1978,  1541,  1665,  1749,  1036,  1037,  1327,  1336,  1543,  1757,
    1812,  1758,  1813,  1806,  1852,  1428,  1596,  1438,  1604,    82,
      83,    84,    85,    86,   174,  1014,   634,   635,   636,  1012,
    1317,  1671,  1672,  1848,  1908,    87,   383,  1009,   629,   630,
     631,  1007,  1312,  1668,  1669,  1847,  1906,    88,    89,    90,
      91,    92,    93,    94,    95
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1411
static const yytype_int16 yypact[] =
{
     836, -1411,   113,   953, -1411, -1411,  -106, -1411, -1411, -1411,
     -50,   110, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,     2, -1411, -1411,    75,    83,   122,   135,   146,
     157,   164,   176,   225,   108,   482, -1411, -1411,   253,   278,
     314,   317,   179,   106,   377, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
     402, -1411,   207, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411,   353, -1411,   413,   161,   430,   440,
     393,   183,   196,   468,   483,   490,   510,   539,   571, -1411,
     256,   602,   601,   349,   383,   384,   400, -1411,   404,   405,
     415,   431,   437,   439,   608,   637,   441,   442,   445,   446,
   -1411, -1411,   462, -1411, -1411,   486,  -160,   255,   484,     1,
     472,   803, -1411,   834, -1411, -1411,    72,   148,    12,   180,
    1050,   842, -1411, -1411, -1411, -1411, -1411,   471, -1411, -1411,
   -1411, -1411, -1411,   470,   479,   481,  1075, -1411,   485,   489,
   -1411, -1411, -1411, -1411,   491, -1411, -1411, -1411, -1411, -1411,
   -1411,   511,   513, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
     494, -1411, -1411,   844,   875,   565,   802,   886,   882,   891,
     805,   682, -1411,   812,   961,   815,   -19,   816, -1411,   817,
     818,   819,    86,   820,   822,   823,    15,   824, -1411,   -82,
     825,   837,   433,   716,   838, -1411,   840, -1411,   841,   843,
   -1411,   846, -1411,   847,   848,   850,   851,   852,   853,   854,
     855,   857,   859,   860,   861,   863,   864,   865, -1411,   218,
     866,   867,   868,   869,   870,   595,   871,   893,   894,   896,
   -1411,   218, -1411,   585,   897,   641,   898, -1411, -1411, -1411,
   -1411, -1411, -1411,   900, -1411, -1411, -1411, -1411,  1036, -1411,
   -1411, -1411,   280, -1411, -1411, -1411, -1411,   907, -1411,   361,
      73,   951, -1411, -1411, -1411,   916,  1031,   923, -1411, -1411,
   -1411,   177, -1411,   927, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411,    50, -1411, -1411,   930,   931, -1411, -1411,  -127, -1411,
     932, -1411,   933,   100, -1411, -1411, -1411,   339,  1038,   328,
    1066, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411,   937, -1411, -1411, -1411, -1411,   615, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411,   941, -1411,   942,
   -1411,  1076, -1411, -1411, -1411, -1411, -1411,   733,   -45,  -109,
     -57,   887, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411,   395,   428,  -109,  -109, -1411,   949,   948,
     950,   952, -1411,   962,   963,   968,   969,   972,   691,    36,
     715, -1411, -1411, -1411, -1411,   717,   718,   976,   719,   -79,
     -71,   125,   721,   723, -1411,   724, -1411, -1411, -1411, -1411,
   -1411, -1411,   727,   728,   989,   730,   734,   999,   742,   743,
   -1411, -1411, -1411,  1002,   228,   747,   227,   748,   227, -1411,
     749,   227,   751,   227, -1411,   752,   755,   756,   757,   758,
     759,   764,   770,   772,   773,  1032,   775,   785,  1047,   294,
   -1411, -1411,  1046,   791, -1411, -1411,   792,   876,   858,     5,
     794,   798,   -64,   800,  -127,  1059,   821,  -127,   804, -1411,
     809,  1062,  1065,   813,  1074,  1086, -1411, -1411,   477,  1064,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411,  1085,   831,   488,
     419,   834,   832,   640, -1411,  1093,  1227,   322, -1411, -1411,
      94,  1094,  1096,  -127,  1098,  1099,  1101,  1102, -1411, -1411,
    1095, -1411, -1411,   -21,   845,   849,   856,  1106,  1265,   -83,
   -1411,  -228,   -13, -1411,   934,   422,    80, -1411, -1411, -1411,
   -1411,   872,  1110,  1111,  1114,   873,  1115,   895,  1117,   899,
    1275,   901,   906,   908,   -77,  1119,   910,   911, -1411, -1411,
   -1411,   913,   517, -1411, -1411,    -9,   917,    13, -1411, -1411,
     -30,   -28,   -25,   -23,  1118,  1254, -1411, -1411,  1127,  1123,
    1123,   427, -1411,   538, -1411,  1123, -1411, -1411,   247,   918,
   -1411, -1411,   -57,   -45,   -45,   289, -1411,   -57,  -109,  -117,
     -57, -1411, -1411,   -57,   -57,   529,   346,   909, -1411,  1124,
    1125,  1126,  1128,  1131,  1136,  1154,  1156,  1133, -1411,   224,
   -1411, -1411,  1155, -1411,   245, -1411, -1411,  -117,  -117,  1158,
     919,   920,   921,   922,   926,   928,   929,   935, -1411,   936,
     915, -1411, -1411, -1411, -1411, -1411,   938, -1411,   939, -1411,
     940,   943, -1411, -1411,   -84, -1411, -1411, -1411,   668,  -107,
   -1411, -1411,   944,  1165, -1411, -1411,   946,  1035,  1167,   947,
    1181, -1411, -1411,   954,   956, -1411,   957,   984, -1411,   958,
   -1411,   959,   984, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411,   960, -1411, -1411,  1188, -1411, -1411, -1411,
   -1411,   964,  1189, -1411,  1195, -1411, -1411,  1191,  1192,  1200,
    1199,  1205,   965, -1411, -1411,   966, -1411, -1411, -1411, -1411,
    1201, -1411,  1211,  -127, -1411, -1411, -1411,   967, -1411,   970,
     677,  -105, -1411,  1209, -1411, -1411,  1208, -1411, -1411,   971,
   -1411,  -127, -1411,   145, -1411, -1411, -1411, -1411,   322,   973,
   -1411, -1411, -1411,   974,   975,   978,   979,  -127,   980,  1368,
    1233,  1212,  1219,  1220, -1411, -1411, -1411, -1411, -1411, -1411,
    1221, -1411, -1411, -1411, -1411, -1411,  -103, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,   987,   988,   990, -1411,   991, -1411, -1411, -1411,
    1226, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,   683,
     -76, -1411, -1411, -1411,  1213,   198, -1411, -1411,   992,  1229,
    1230, -1411, -1411,  1247,   185,   501, -1411,  1251,  1252, -1411,
    1255,  1257, -1411,  1258,  1259, -1411, -1411, -1411,  1260,  1261,
    1262,  1263,  1264,  1266,  1267,   492,   171,  1268,   839,  1269,
    1270, -1411, -1411,  1256,  1271,  1272,  1273,  1274,  1276,  1277,
    1278,  1279,  1280,  1281,  1282,  1283,  1284, -1411, -1411, -1411,
   -1411,   -85, -1411, -1411, -1411,   403,  1287,  -127, -1411,  -127,
    -127,  -127,  -127,  -127, -1411,   997, -1411,   796,  1289, -1411,
    1286,  1288,  1290,  1291, -1411, -1411,  1292,  1293,  1294,  1295,
   -1411,   478,  1241, -1411, -1411,  1296,  1028,  1039, -1411, -1411,
    1299,  1300,  1301, -1411, -1411, -1411, -1411, -1411,  1235,   576,
      66,  1302,  1043,  1045,  1304,  1305,  1048,  1307,  1308,  1051,
      19,  1052,  1313,  1315, -1411, -1411, -1411, -1411,   -10,   220,
   -1411,   107,   -45,   -45,   -45,   -45, -1411, -1411, -1411,   220,
     114,  -109, -1411,   220, -1411,   784,    43,   662, -1411, -1411,
   -1411, -1411, -1411, -1411,   -45,   -45, -1411, -1411, -1411,   -45,
     -57,   -57,   -57,   -57, -1411,  -109,  -109,  -109,   -32,   -32,
     -32,   -32,   -32,   -32,   -32,   -32,  1056,  1316,  1243,   137,
   -1411,  1060,  1317,  1244,   137, -1411, -1411, -1411,  1061, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,  1063, -1411,
   -1411, -1411,  1068, -1411, -1411, -1411,     4,   -84, -1411, -1411,
   -1411, -1411, -1411,  1321,   290, -1411,  1069, -1411,  1323, -1411,
   -1411, -1411, -1411,  -127, -1411, -1411, -1411,  1067, -1411, -1411,
    1070, -1411, -1411, -1411, -1411,  1324,  1072,   -61,  1336,  1370,
    1333, -1411,  1334, -1411, -1411,   -46, -1411,  1335,  1134, -1411,
   -1411,  1137, -1411, -1411, -1411, -1411, -1411, -1411,  1077, -1411,
    -127,  -127,  1227, -1411,   -75, -1411,  1341, -1411, -1411, -1411,
   -1411,  1082, -1411,  1342,  1343,  1083,   -98, -1411,  1087,  1135,
   -1411, -1411, -1411, -1411, -1411, -1411,  1088, -1411, -1411, -1411,
   -1411, -1411,  1089, -1411, -1411, -1411, -1411,  1135,  1090,  1091,
    1092,  1097,  1100,  1103,  -110,  1346,  1104,  1105,  1351,  1354,
    1107,  1355,  1108,  1109,  1356,  1357,  1112,  1113,  1116,  1120,
    1121,  1122,  1129, -1411, -1411, -1411, -1411,  1130, -1411, -1411,
   -1411,  1132,  1138, -1411, -1411, -1411, -1411, -1411, -1411,  1139,
    1140,  1141,  1358,  1359,  1142,  1468,  1468,  1468,  1143,  1144,
    1468,  1468,  1468,  1468,  1472,  1472,  1468,  1472,   321,  1371,
    1369,  1374,     0, -1411,   753,    50, -1411,   796, -1411, -1411,
       8,  -127, -1411,  -127,  -127,  1145, -1411,  1375, -1411, -1411,
   -1411,  1376,  1377,  1381,  1382,  1385,  1384,  1386,  1389,  1391,
    1387, -1411, -1411, -1411,  1146,  1392, -1411,    -7,  1393,   300,
   -1411, -1411,  1147,  1148,  1149,  1394, -1411, -1411, -1411,   613,
    1395, -1411, -1411,  1424, -1411, -1411,  1513,  1547, -1411,  1548,
    1549, -1411,  1383,  1412, -1411, -1411, -1411, -1411,  1123,  1123,
   -1411,   736, -1411,  1416,  1417,  1418, -1411, -1411,  1552, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411,  1378,   -45, -1411,   316,
     316, -1411, -1411,  -109, -1411, -1411,   -57, -1411,   358,   358,
     358, -1411, -1411,   537,   337,  1166,  1166,  1166,  1420, -1411,
   -1411,  1423,  1164,  1168,  1169,  1170,  1171,  1172,  1173,  1174,
   -1411, -1411,    -2, -1411, -1411, -1411, -1411,    42, -1411, -1411,
   -1411,  1425,   325, -1411,  1433,   330, -1411,  1176,  1434, -1411,
    1435,  1436,  1437,  1438,  1439,  1440,  1183, -1411,  1442,  1185,
   -1411,  1186, -1411, -1411, -1411,  -127, -1411, -1411,  1248,  -119,
   -1411, -1411, -1411, -1411,  1446, -1411,  1447, -1411, -1411, -1411,
    1448,  1449,  1253, -1411,  1203, -1411, -1411,  1429, -1411,  1452,
   -1411,  1453, -1411,  -127,  1196, -1411, -1411, -1411, -1411, -1411,
    1197,  1198, -1411,  1202, -1411, -1411, -1411, -1411, -1411,  1595,
   -1411, -1411, -1411, -1411, -1411, -1411,  1204, -1411, -1411, -1411,
   -1411,  1458,  1206, -1411,  1207, -1411, -1411,  1210,  1459, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,   806,   -74, -1411,  1461, -1411, -1411,  1214,  1215,
    1216, -1411, -1411,  1217,  1218,  1222,  1223, -1411,  1224,  1225,
    1228,  1231, -1411, -1411, -1411, -1411,  1232,  1234,  1236,  1237,
   -1411,   874, -1411,  1238,  1239,  1598,  1240,   -16, -1411,  -127,
   -1411,  1604, -1411,  1469,  1242,  1245,  1246,  1249,  1250, -1411,
     -59,  1285,  1297,  1298,  1303, -1411,   -52, -1411,  1443, -1411,
   -1411,  1470, -1411, -1411, -1411, -1411, -1411,  1471, -1411, -1411,
   -1411,  1415,  1477,   478,  1484,  1485,  1487,  1488, -1411,  1466,
    1306,  1309,  1493, -1411, -1411, -1411,   348,  1499,  1310,  1311,
    1495, -1411, -1411,   117,    54,   547,   -29,   516, -1411,  1450,
    1450, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411,  1422, -1411,  1426,  1502,  1312, -1411,  1506,  1314, -1411,
   -1411, -1411, -1411,  1511,  1510,  1451,  1318,  1319,  1320, -1411,
   -1411,  1322, -1411, -1411, -1411,    46,    47, -1411,  1512,   447,
    1517,  1518,  1325, -1411, -1411,  1326, -1411,  1519,  1520,  1521,
    1522,  1523,  1327,  1328,   -68,  1524,  -127, -1411, -1411, -1411,
   -1411, -1411,  1574,  1525, -1411,  1329, -1411, -1411, -1411,  1330,
   -1411, -1411, -1411, -1411, -1411,  1331,  1528, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411,  1529, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411,  1332, -1411, -1411, -1411, -1411,    77,
    1530,  1337, -1411,    17,  1338,  1604, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,  1531,
    1532,  1533,  1534, -1411,  1474,  1563,  1569,  1570,  1571,  1538,
   -1411, -1411, -1411, -1411,   -58,  1339, -1411, -1411, -1411, -1411,
    1498,   -45,  -109,   -57,  1540,  1541,  1541,  1542,  1422, -1411,
    1544,  1426, -1411,  1340, -1411,  1344, -1411, -1411, -1411,  1545,
    1546,  1550,  1551, -1411,    49, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,  1366, -1411, -1411, -1411, -1411,  1352,  1586,  1557,
   -1411, -1411, -1411, -1411,  1553,  1345, -1411, -1411,  1347, -1411,
    -127, -1411,  1713, -1411, -1411, -1411, -1411, -1411, -1411,  1558,
    1559,  1348,  1719, -1411,   -16, -1411, -1411,  1349,    25,  1350,
    1353,  1562,  1565,  1566,  1573,  1575,  1576,  1577,  1360,  1579,
   -1411, -1411, -1411, -1411,   358,  1166,   376,  1578, -1411, -1411,
   -1411,  1361, -1411,  1362, -1411, -1411, -1411,  1607,    21,  1580,
   -1411, -1411,  1367, -1411,  1535,  1535,   -56,  1581,  1363,  1364,
    1592,  1365,  1352,  1593,  1594, -1411,  1617, -1411,  1372,    40,
    -127,  1574,  1599, -1411, -1411, -1411,  1602,  1373, -1411,  1379,
   -1411, -1411,  1608,  1612,  1611,  1613,  1380,  1388, -1411, -1411,
   -1411,  1614, -1411, -1411, -1411, -1411, -1411,  1615,  1616, -1411,
   -1411, -1411,  1390,  1396,  1622,   -69,  1624, -1411, -1411, -1411,
    1428, -1411,  1626, -1411,  1655, -1411, -1411, -1411, -1411,  1520,
    1627, -1411, -1411, -1411,  1629, -1411,  1664,  1666, -1411, -1411,
    1397,  1633,  1634,  1635,  1744,  1747,  1398,  1560,  1567,  1582,
    1643, -1411, -1411,  1644, -1411, -1411,  1647,  1648,  1596, -1411,
    1649, -1411,  1650, -1411, -1411,  1656, -1411,  1659,  1400,  1653,
    1654, -1411,  1660,  1661,  1401, -1411, -1411, -1411,  1662,  1663,
    1665, -1411,  1467, -1411, -1411,  1667, -1411, -1411, -1411,  1671,
    1520,  1668, -1411,  1656,  1441,  1673, -1411,  1669,  1672,  1674,
    1419, -1411, -1411, -1411, -1411, -1411,    51, -1411,    52,  1675,
    1676, -1411, -1411, -1411,    53,  1678, -1411, -1411,  1203, -1411,
    1679,  1427,  1441,  1682, -1411, -1411,  1681, -1411,  1684,  1684,
      45, -1411, -1411, -1411, -1411, -1411, -1411,    57,    60, -1411,
   -1411,  1476,  1688,  1686,  1722, -1411, -1411,  1430,  1687, -1411,
   -1411,  1836,  1690, -1411,  1739,  1692,  1693,  1694, -1411,  1454,
   -1411, -1411,    61,  1695,  1696, -1411, -1411,  1697,  1496, -1411,
    1699,  1444,  1702,  1701,  1455,  1456,  1457, -1411, -1411, -1411,
    1742, -1411, -1411, -1411,  1705,  1460, -1411, -1411,  1462, -1411,
   -1411, -1411,  1706, -1411,  1520,  1707, -1411, -1411, -1411,  1520,
    1463,  1539,  1708,  1709,  1710,  1711,  1464, -1411, -1411,   -17,
    1712,  1717, -1411,  1723, -1411,  1724,  1725,  1721,  1726,  1727,
    1473,  1475,  1478,  1728, -1411, -1411, -1411,  1479, -1411
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411,   -20, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411,   129, -1411,  1645, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,   173,
   -1411,   216, -1411,  1399, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,   142, -1411, -1334, -1411,  1402, -1411,   877, -1411,
     588, -1411,   586, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
      20, -1411, -1411,    -6, -1411, -1411, -1411, -1411,   977, -1411,
    1625, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,  1431, -1411, -1411,  1175, -1411,  -128, -1411,  -328,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411,  1404,  1177, -1411,  -508, -1411, -1411,
    -131, -1411, -1411, -1411, -1411, -1411, -1411,   826, -1411,   981,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1406, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,  -282, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411,  1150, -1411,   722, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411,   497, -1411, -1411, -1411, -1411,
   -1411, -1411,   725,  1016, -1411, -1411, -1411, -1411, -1411,  -564,
    -895,  -894, -1411, -1411, -1411, -1410,  -570, -1411, -1411, -1411,
   -1411, -1411,    22, -1411, -1411, -1411, -1411, -1411, -1411,  1151,
   -1411, -1411, -1411,   143, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
     697, -1411, -1411,   199,   435, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411,  1779, -1411, -1411, -1411,  -585,  -556,  -867,  -361,  -599,
    -363,  -615, -1411, -1411, -1411, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411,  -199, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411,   411,   266,   903, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411,  -540, -1411,  -949, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411, -1411, -1411,  1403, -1411, -1411,
   -1411, -1411,   262, -1411, -1411, -1411, -1411, -1411, -1411,  1405,
   -1411, -1411, -1411, -1411,   268, -1411, -1411, -1411, -1411, -1411,
   -1411, -1411, -1411, -1411, -1411
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -726
static const yytype_int16 yytable[] =
{
     509,   571,   997,   959,  1200,  1203,   609,   605,   969,   762,
    1556,   973,   288,  1328,   974,   976,   898,   900,  1620,   942,
     943,   329,   637,   638,   972,   951,   719,   843,   301,  1568,
     269,   994,   897,   789,   899,   401,   557,   901,  1383,   903,
     844,  1033,  1807,   416,  1396,   489,   845,   572,  1560,   302,
     846,  1624,  1016,  1017,  1260,  1192,  1252,   847,   848,  1951,
     541,   271,   606,   668,   909,   740,    97,  1109,  1329,  2010,
     849,  2011,   542,   543,   544,   191,   850,  1352,   851,   852,
    1952,  2012,  1953,   853,   854,   781,   782,   489,   545,   855,
     425,   824,   825,   656,   829,   513,  1422,  1719,  1856,   783,
    1653,   658,   607,   856,   489,   826,   520,   417,   725,   291,
     561,  1631,  1261,     4,   606,   601,   489,   857,  1637,   858,
     790,   859,   100,   860,    14,  1240,   489,   601,   787,   861,
     862,   791,   792,   863,   864,   804,   805,   418,   915,   865,
     866,   402,   489,   303,   489,   521,   546,   489,   763,   489,
     562,   773,    16,   304,   610,  1954,   489,   611,   612,   867,
    1261,   514,   547,   840,   522,  1479,   602,   613,   788,   806,
    1530,   868,   489,   110,   869,   426,  1158,   870,  1262,  1955,
     489,   749,   523,   524,  1659,   296,   807,   990,   991,   489,
     133,  1129,   728,   992,   993,   733,   311,   292,   312,   534,
    1298,  1299,  1300,   101,   102,   419,   134,   525,   649,   142,
     411,  1720,   489,   808,  1532,  1727,   313,  1159,  1685,  1685,
    1684,  1685,  1253,  1931,  1933,  1685,  1262,  1857,   271,  1685,
     314,   315,  1685,  1685,  1561,  1123,  1439,  1301,  1441,  1330,
     526,   767,   960,   961,   838,   970,   316,   112,  1741,   871,
    1093,   977,   420,   975,   764,   113,  1808,   141,   412,   990,
     991,  1008,   568,   872,   317,   992,   993,  1659,   318,   193,
     192,   142,   873,   535,   874,   875,   876,   877,   878,   121,
     319,   194,  1013,  1160,   953,   195,   320,   548,  1034,   563,
     297,  1241,   196,   650,   114,  1242,  1130,   660,   728,   728,
     902,   904,   793,  1331,   507,  1457,  1459,   115,  1460,   971,
     321,   322,   323,   966,  1787,   513,  1956,   967,   116,  1809,
    1397,   324,   608,  1041,  1353,  1085,  1354,  1110,   325,   117,
    1659,  1355,  1384,  1199,  1035,  1201,   118,   879,   880,   881,
     882,   883,   884,   885,   886,  1193,   507,   784,   119,   427,
    1332,   657,  1356,   827,  1120,  1376,  1593,   403,   404,   659,
     887,   197,   997,   507,  1705,   198,   726,   421,   603,  1357,
     720,  1632,  1740,   549,   614,   507,  1819,  2013,  1638,  1263,
     603,   514,   661,   270,  1365,   507,   604,   794,  1264,  1265,
     151,  1291,  1292,  1293,  1294,   678,   888,   120,   550,   682,
     679,   507,   199,   507,   889,  1660,   507,   305,   507,   774,
     890,  1333,  1334,  1283,   536,   507,   326,  1286,  1335,  1195,
     200,   841,   527,  1480,  1314,   126,  1957,   721,  1531,  1319,
    1450,   507,  1275,   797,   798,   809,   990,   991,  1458,   507,
    1196,  1829,   992,   993,   954,   955,   327,  1725,   507,   891,
     127,  1079,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,  1091,
    1833,   507,  1533,   627,   628,  1287,  1686,  1687,  1576,  1763,
     971,  1932,  1934,  1940,   683,  1101,   128,  1960,  1660,   129,
    1961,  1979,   513,   203,   632,   633,   130,   131,   204,   205,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   206,  1261,   207,
     271,   208,   750,    16,   680,   271,  1373,   799,   751,   800,
    1890,   513,   962,   963,   964,   965,   272,   209,  1221,  1278,
     971,   272,   962,   963,   964,   965,  1284,  1131,  1153,   136,
     201,  1660,  1914,   990,   991,   328,  1132,  1133,   514,   992,
    1222,  1223,   990,   991,  1622,   157,  1623,   966,   992,   993,
     210,   967,   139,   801,   802,   122,   123,  1937,  1938,   188,
     189,  1154,   273,   153,  1262,   211,  1377,   273,   460,   461,
    1134,   154,   990,   991,   212,   944,   945,   514,   992,   993,
     155,  1279,  1280,  1281,  1282,  1962,   566,   567,  1285,  1194,
     156,   686,   213,   158,   689,   728,   691,   728,   214,  1204,
    1205,  1206,   288,  1288,  1289,   274,   159,   215,  1290,  1155,
     274,  1156,  1295,  1296,  1297,   275,  1429,  1430,   160,   216,
    1433,  1434,  1435,  1436,   627,   628,  1440,  1662,  1994,  1663,
    1662,   217,   578,   161,  1276,   707,   708,   709,   710,  1999,
     162,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   632,   633,   579,
     163,  1709,  1442,  1443,  1444,  1445,   167,  1516,  1500,  1501,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   947,   948,   276,   164,
     506,   507,   580,   276,   962,   963,   964,   965,   581,   966,
     218,  1043,  1339,   967,   582,   430,   431,   432,  1724,  1659,
    1482,  1228,   978,   979,   980,   981,   982,   983,   995,   996,
     219,   165,   220,   964,   965,   978,   979,   980,   981,   982,
     983,   984,   985,   990,   991,   221,  1321,  1535,   570,   507,
    1659,  1324,  1538,   978,   979,   980,   981,   982,   983,   984,
     985,  1345,   168,   169,   222,   223,   966,   583,   181,   170,
     967,  1386,   584,   962,   963,   964,   965,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,  1690,  1691,  1389,
    1303,  1304,  1305,  1306,  1307,  1308,  1309,   182,   728,  1374,
     755,   756,   757,   171,   172,  1779,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     173,   248,   285,   249,   175,   176,    -7,     1,  1038,  1039,
    1040,   250,   251,   252,    -7,   177,  1163,  1082,  1083,  1084,
     253,   254,   255,  1117,  1118,  1119,  1164,  1236,  1237,  1238,
     579,   178,   256,   257,   258,   259,  1453,   179,    -7,   180,
    1165,   183,   184,    -7,   260,   185,   186,   261,   978,   979,
     980,   981,   982,   983,   995,   996,   262,   263,   264,   265,
     266,  1166,   187,   580,  1488,  1489,  1490,    -7,   142,   581,
      -7,   366,   997,   368,    -7,   582,  1503,  1504,  1505,   365,
    1451,    -7,   369,    -7,   370,   381,  1513,    -7,   748,   382,
    1514,   384,  1461,  1517,   387,  1515,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   971,   389,   385,  1167,   386,   837,   390,   986,
    1660,   987,   988,   391,   962,   963,   964,   965,   583,   392,
     393,     5,   986,   584,   987,   988,    -7,   394,  1168,   395,
      -7,   396,   962,   963,   964,   965,  1590,  1591,  1592,   397,
     986,  1660,   987,   988,   398,     6,   399,   400,   406,   408,
     409,   410,   413,    -7,   414,   415,   423,   428,    -7,    -7,
     978,   979,   980,   981,   982,   983,   984,   985,   434,   429,
     435,   468,   437,   439,     7,   440,   477,     8,   442,   444,
     445,     9,   446,   447,   448,   449,   450,   451,    10,   452,
      11,   453,   454,   455,    12,   456,   457,   458,   463,   464,
     465,   466,   467,   470,    -7,    -7,    -7,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,  1557,  1746,   471,   472,    -7,   473,   478,
     480,   479,   483,   486,    -7,    -7,    -7,   512,    -7,    -7,
      -7,    -7,    -7,    13,    -7,   529,   530,    14,   971,    -7,
      -7,    -7,   531,   532,  1284,   986,    -7,   987,   988,   538,
     554,   555,   568,   573,   559,   560,    -7,   575,    -7,    -7,
      15,   592,   371,   596,   594,    16,    17,   599,   618,   639,
     640,   648,   641,  1614,   642,   978,   979,   980,   981,   982,
     983,   995,   996,   372,   643,   644,    -7,    -7,    -7,   373,
     645,   646,   374,   375,   647,   651,   654,   652,   653,   655,
      -7,   662,    -7,   663,   665,    -7,   376,   666,   667,   668,
     671,    18,    19,    20,   908,   672,   909,   910,   911,   912,
     913,   673,   674,   675,   676,   377,   914,   681,   685,   688,
     353,   690,   693,  1452,    21,   694,   695,   696,   697,   698,
      -7,    22,    23,    24,   699,    25,    26,    27,    28,    29,
     700,    30,   701,   702,   703,   704,    31,    32,    33,   962,
     963,   964,   965,    34,   706,   705,  1278,   986,   712,   987,
     988,   713,   716,    35,   723,    36,    37,   718,   724,   717,
     727,   730,   736,   731,   734,   737,   745,   378,   379,   735,
     915,   916,   917,   738,   739,   918,   919,   354,   355,   356,
     357,   358,   359,    38,    39,    40,   740,   746,    18,    19,
      20,   747,   754,   759,   760,   772,   765,    41,   766,    42,
     768,   769,    43,   770,   771,   776,   779,   920,   780,   777,
     812,   813,   796,   921,   814,   816,   778,   818,   820,   829,
     905,   906,  1707,   941,   998,   999,  1000,  1056,  1001,  1745,
    1744,  1002,   811,   815,  1613,  1006,  1003,    44,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,  1004,   817,  1005,  1011,  1018,   819,
    1048,   821,   922,   923,   924,   971,   822,  1046,   823,  1050,
     832,   833,   986,   836,   987,   988,  1028,   842,   957,  1019,
    1020,  1021,  1022,  1052,   925,   926,  1023,   927,  1024,  1025,
    1062,  1064,  1065,  1068,  1069,  1026,  1027,  1070,  1029,  1030,
    1031,  1071,  1072,  1076,  1032,  1043,  1047,  1051,  1077,  1087,
    1088,  1103,  1104,  1122,  1105,  1053,  1054,  1055,  1058,  1059,
    1061,  1106,  1107,  1108,  1063,  1073,  1074,  1080,  1116,  1126,
    1081,  1089,  1127,  1095,  1097,  1098,   928,   929,  1099,  1100,
    1102,   930,   931,   932,   933,   934,   935,  1112,  1113,  1128,
    1114,  1115,  1125,  1136,  1137,  1139,  1174,  1208,  1780,  1140,
    1142,  1143,  1146,  1147,  1148,  1149,  1150,  1210,  1151,  1152,
    1162,  1170,  1171,  1175,  1176,  1177,  1178,  1225,  1179,  1180,
    1181,  1182,  1183,  1184,  1185,  1186,  1187,  1198,  1211,  1228,
    1212,  1235,  1213,  1214,  1216,  1217,  1218,  1219,  1226,  1231,
    1232,  1233,  1234,  1244,   840,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1255,  1256,  1707,  1257,  1310,  1313,  1311,  1316,
    1315,  1320,  1318,  1338,  1321,  1342,  1350,  1346,  1834,  1324,
    1347,  1341,  1351,  1360,  1361,  1362,  1364,  1368,  1369,  1372,
    1371,  1378,  1379,  1382,  1380,  1381,  1398,  1385,  1387,  1388,
    1390,  1391,  1392,  1401,  1402,  1404,  1407,  1393,  1422,  1408,
    1394,  1425,  1427,  1395,  1399,  1400,  1437,  1403,  1405,  1406,
    1448,  1447,  1409,  1410,  1449,  1464,  1411,  1494,  1465,  1466,
    1412,  1413,  1414,  1467,  1468,  1469,  1471,  1475,  1472,  1415,
    1416,  1473,  1417,  1474,  1478,  1481,  1487,  1492,  1418,  1419,
    1420,  1421,  1426,  1431,  1432,  1462,  1477,  1484,  1485,  1486,
    1493,  1495,  1496,  1497,  1499,  1498,  1507,  1508,  1509,  1510,
    1512,  1518,   971,  1521,  1522,  1540,  1571,  1534,  1523,  1524,
    1525,  1526,  1527,  1528,  1529,  1537,  1542,  1544,  1545,  1546,
    1547,  1548,  1549,  1550,  1551,  1552,  1553,  1558,  1563,  1564,
    1565,  1566,  1567,  1569,  1573,  1575,  1577,  1578,  1579,  1583,
    1585,  1589,  1580,  1595,  1584,  1617,  1586,  1587,  1620,  1625,
    1588,  1642,  1640,  1641,  1597,  1598,  1599,  1600,  1601,  1643,
    1639,  1618,  1602,  1603,  1605,  1606,  1645,  1646,  1607,  1647,
    1648,  1608,  1609,  1652,  1610,  1658,  1611,  1612,  1615,  1616,
    1649,  1655,  1626,  1667,  1673,  1627,  1628,  1670,  1675,  1629,
    1630,  1677,  1678,  1710,  1689,  1664,  1679,  1680,  1681,  1693,
    1694,  1698,  1685,  1700,  1701,  1702,  1708,  1712,  1716,  1717,
    1733,  1734,  1722,  1728,  1729,  1730,  1731,  1735,  1736,  1737,
    1738,  1743,  1747,  1748,  1751,  1633,  1753,  1759,  1760,  1767,
    1773,  1770,  1761,  1762,  1774,  1776,  1782,  1634,  1635,  1704,
    1783,  1784,  1786,  1636,  1792,  1682,  1650,  1793,  1795,  1651,
    1656,  1657,  1674,  1805,  1676,  1796,  1801,  1797,  1798,  1799,
    1802,  1830,  1811,  1820,  1683,  1695,  1696,  1703,  1832,  1713,
    1714,  1715,  1718,  1816,  1824,  1827,  1828,  1723,  1726,  1742,
    1842,  1836,  1755,  1822,  1837,  1777,  1756,  1778,  1785,  1788,
    1840,  1814,  1790,  1791,  1841,  1843,  1846,  1849,  1850,  1862,
    1800,  1803,  1804,  1821,  1855,  1825,  1859,  1860,  1861,  1866,
    1851,  1868,  1869,  1838,  1870,  1872,  1873,  1874,  1875,  1839,
    -724,  1876,  1878,  1853,  1879,  1881,  1882,  1880,  -725,  1883,
    1884,  1887,  1888,  1891,  1885,  1897,  1898,  1871,  1877,  1895,
    1896,  1901,  1899,  1900,  1905,  1907,  1910,  1909,  1915,  1913,
    1918,  1924,  1920,  1923,  1925,  1963,  1926,  1935,  1936,  1927,
    1941,  1944,  1947,  1948,  1949,  1964,  1967,  1945,  1966,  1969,
    1968,  1970,  1971,  1973,  1974,  1975,  1976,  1980,  1981,  1983,
    1984,  1985,  1987,  1988,  1986,  1977,  1992,  1995,  1998,  2000,
    2003,  2004,  2005,  2006,  2015,  1989,  1990,  1991,  2002,  2016,
    1996,  2021,  1997,  2001,  2007,  2018,  2019,  2020,  2022,  2023,
    2027,  1826,  1946,  2024,  1766,  2025,   475,  1818,  2026,  2028,
    1536,  1539,  1943,  1919,   515,   753,  1086,  1454,  1375,   744,
    1594,  1340,  1455,  1209,  1835,  1929,  1483,  1789,  1644,   364,
    1092,  1666,  1750,  1754,     0,  1266,  1752,     0,     0,  1267,
    1337,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1121,     0,     0,     0,     0,     0,  1124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1010,     0,     0,  1015,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1042,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1060
};

static const yytype_int16 yycheck[] =
{
     282,   329,   617,   602,   898,   900,   369,   368,   607,   517,
    1344,   610,   143,     9,   613,   614,   580,   581,    34,   589,
     590,   149,   385,   386,   609,   595,    21,    14,    16,  1363,
      29,   616,    62,    46,    62,    54,   318,    62,   136,    62,
      27,   125,    21,    28,   154,   172,    33,   329,   167,    37,
      37,  1461,   637,   638,    64,   140,    37,    44,    45,    14,
      10,    48,   171,   170,    39,   170,   172,   170,    64,    86,
      57,    88,    22,    23,    24,   235,    63,   138,    65,    66,
      35,    98,    37,    70,    71,   168,   169,   172,    38,    76,
     172,   168,   169,   172,   170,   170,   170,    20,   167,   182,
    1506,   172,   211,    90,   172,   182,    33,    92,   172,    37,
      10,   170,   170,     0,   171,   172,   172,   104,   170,   106,
     133,   108,   172,   110,   134,    59,   172,   172,   356,   116,
     117,   144,   145,   120,   121,    55,    56,   122,   113,   126,
     127,   160,   172,   131,   172,    72,    96,   172,    54,   172,
      50,   172,   162,   141,   211,   110,   172,   214,   215,   146,
     170,   236,   112,   172,    91,   172,   211,   224,   396,    89,
     172,   158,   172,   171,   161,   257,     5,   164,   236,   134,
     172,   509,   109,   110,   213,    37,   106,   216,   217,   172,
      84,     6,   474,   222,   223,   477,    16,   125,    18,    22,
     232,   233,   234,    93,    94,   190,   100,   134,   172,    64,
     124,   134,   172,   133,   172,  1625,    36,    46,   172,   172,
    1554,   172,   203,   172,   172,   172,   236,   296,    48,   172,
      50,    51,   172,   172,   353,    37,  1185,   269,  1187,   235,
     167,   523,   603,   604,   572,   608,    66,   172,  1654,   236,
     758,   614,   237,   614,   160,   172,   235,    50,   172,   216,
     217,    37,    64,   250,    84,   222,   223,   213,    88,    14,
     430,    64,   259,    96,   261,   262,   263,   264,   265,   171,
     100,    26,    37,   112,    37,    30,   106,   237,   372,   189,
     142,   225,    37,   257,   172,   229,   111,   172,   580,   581,
     582,   583,   315,   299,   431,  1199,  1201,   172,  1203,   426,
     130,   131,   132,   430,  1724,   170,   271,   434,   172,   298,
     430,   141,   431,   430,   385,   430,   387,   430,   148,   172,
     213,   392,   430,   897,   418,   899,   172,   324,   325,   326,
     327,   328,   329,   330,   331,   430,   431,   430,   172,   431,
     346,   430,   413,   430,   430,   430,   430,   376,   377,   430,
     347,   106,   977,   431,   432,   110,   430,   352,   425,   430,
     365,   430,   430,   323,   431,   431,   432,   394,   430,   389,
     425,   236,   257,   382,   430,   431,   431,   400,   398,   399,
      37,   990,   991,   992,   993,   167,   383,   172,   348,   172,
     172,   431,   147,   431,   391,   434,   431,   395,   431,   430,
     397,   407,   408,   969,   237,   431,   236,   973,   414,    16,
     165,   430,   349,   430,  1009,   172,   381,   422,   430,  1014,
     430,   431,   212,    11,    12,   355,   216,   217,   430,   431,
      37,  1775,   222,   223,   197,   198,   266,   430,   431,   577,
     172,   733,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   751,
     430,   431,   430,   249,   250,   432,   430,   430,  1373,   430,
     426,   430,   430,   430,   257,   767,   172,   430,   434,   172,
     430,   430,   170,     9,   249,   250,   317,   318,    14,    15,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,    33,   170,    35,
      48,    37,   103,   162,   296,    48,  1090,   105,   109,   107,
    1864,   170,   425,   426,   427,   428,    64,    53,    60,   432,
     426,    64,   425,   426,   427,   428,   432,    46,    56,   172,
     295,   434,  1886,   216,   217,   375,    55,    56,   236,   222,
      82,    83,   216,   217,  1459,   172,  1460,   430,   222,   223,
      86,   434,   170,   151,   152,    93,    94,  1911,  1912,    93,
      94,    89,   110,   170,   236,   101,  1094,   110,   370,   371,
      89,   430,   216,   217,   110,   168,   169,   236,   222,   223,
     170,   962,   963,   964,   965,  1939,   267,   268,   971,   891,
     170,   438,   128,   430,   441,   897,   443,   899,   134,   901,
     902,   903,   753,   984,   985,   153,   430,   143,   989,   137,
     153,   139,   995,   996,   997,   163,  1176,  1177,   170,   155,
    1180,  1181,  1182,  1183,   249,   250,  1186,  1514,  1982,  1516,
    1517,   167,    37,   170,   434,   361,   362,   363,   364,  1993,
     170,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   249,   250,    64,
     170,  1576,   361,   362,   363,   364,   430,  1286,  1258,  1259,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   168,   169,   236,   170,
     430,   431,    97,   236,   425,   426,   427,   428,   103,   430,
     236,   431,   432,   434,   109,   292,   293,   294,  1622,   213,
     430,   431,   216,   217,   218,   219,   220,   221,   222,   223,
     256,   170,   258,   427,   428,   216,   217,   218,   219,   220,
     221,   222,   223,   216,   217,   271,   431,   432,   430,   431,
     213,   431,   432,   216,   217,   218,   219,   220,   221,   222,
     223,  1053,   170,   172,   290,   291,   430,   162,   170,   430,
     434,  1109,   167,   425,   426,   427,   428,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   350,   351,  1127,
     999,  1000,  1001,  1002,  1003,  1004,  1005,   170,  1090,  1091,
     170,   171,   172,   430,   430,  1709,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     430,   347,    29,   349,   430,   430,     0,     1,   170,   171,
     172,   357,   358,   359,     8,   430,     7,   170,   171,   172,
     366,   367,   368,   170,   171,   172,    17,   281,   282,   283,
      64,   430,   378,   379,   380,   381,  1194,   430,    32,   430,
      31,   430,   430,    37,   390,   430,   430,   393,   216,   217,
     218,   219,   220,   221,   222,   223,   402,   403,   404,   405,
     406,    52,   430,    97,   281,   282,   283,    61,    64,   103,
      64,   430,  1517,   433,    68,   109,   170,   171,   172,    67,
    1192,    75,   433,    77,   433,   430,  1277,    81,   430,   430,
    1283,   430,  1204,  1286,   430,  1286,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   426,    99,   433,   106,   433,   430,    73,   433,
     434,   435,   436,   388,   425,   426,   427,   428,   162,   157,
      74,     8,   433,   167,   435,   436,   130,    85,   129,    78,
     134,   166,   425,   426,   427,   428,   170,   171,   172,   297,
     433,   434,   435,   436,   172,    32,    25,   172,   172,   172,
     172,   172,   172,   157,   172,   172,   172,   172,   162,   163,
     216,   217,   218,   219,   220,   221,   222,   223,   292,   172,
     172,   416,   172,   172,    61,   172,   431,    64,   172,   172,
     172,    68,   172,   172,   172,   172,   172,   172,    75,   172,
      77,   172,   172,   172,    81,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   208,   209,   210,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,  1345,  1663,   172,   172,   231,   172,   172,
     172,   430,   172,    37,   238,   239,   240,   170,   242,   243,
     244,   245,   246,   130,   248,   134,   170,   134,   426,   253,
     254,   255,    61,   170,   432,   433,   260,   435,   436,   172,
     170,   170,    64,    37,   172,   172,   270,   170,   272,   273,
     157,   170,    37,    37,   172,   162,   163,   384,   231,   170,
     172,   430,   172,  1451,   172,   216,   217,   218,   219,   220,
     221,   222,   223,    58,   172,   172,   300,   301,   302,    64,
     172,   172,    67,    68,   172,   430,   170,   430,   430,   430,
     314,   430,   316,   430,   430,   319,    81,   430,   430,   170,
     430,   208,   209,   210,    37,   431,    39,    40,    41,    42,
      43,   172,   430,   430,   172,   100,    49,   430,   430,   430,
     130,   430,   430,   430,   231,   430,   430,   430,   430,   430,
     354,   238,   239,   240,   430,   242,   243,   244,   245,   246,
     430,   248,   430,   430,   172,   430,   253,   254,   255,   425,
     426,   427,   428,   260,   167,   430,   432,   433,   172,   435,
     436,   430,   430,   270,   430,   272,   273,   369,   430,   353,
     430,   172,   170,   412,   430,   170,   172,   162,   163,   430,
     113,   114,   115,   430,   170,   118,   119,   197,   198,   199,
     200,   201,   202,   300,   301,   302,   170,   172,   208,   209,
     210,   430,   430,   170,    37,   170,   172,   314,   172,   316,
     172,   172,   319,   172,   172,   430,   170,   150,    13,   430,
     170,   170,   348,   156,   170,   170,   430,   170,    13,   170,
     172,    37,  1574,   170,   170,   170,   170,   313,   170,  1662,
    1661,   170,   430,   430,   430,   172,   170,   354,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   170,   430,   170,   172,   170,   430,
     295,   430,   205,   206,   207,   426,   430,   172,   430,   172,
     430,   430,   433,   430,   435,   436,   431,   430,   430,   430,
     430,   430,   430,   172,   227,   228,   430,   230,   430,   430,
     172,   172,   167,   172,   172,   430,   430,   167,   430,   430,
     430,   172,   167,   172,   431,   431,   430,   430,   167,   170,
     172,    13,   149,   170,   172,   431,   430,   430,   430,   430,
     430,   172,   172,   172,   430,   430,   430,   430,   172,   170,
     430,   430,   172,   430,   430,   430,   279,   280,   430,   430,
     430,   284,   285,   286,   287,   288,   289,   430,   430,   172,
     430,   430,   430,   172,   172,   170,   170,   430,  1710,   172,
     172,   172,   172,   172,   172,   172,   172,   148,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   206,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   170,   172,   431,
     172,   226,   172,   172,   172,   172,   172,   172,   172,   430,
     171,   171,   171,   430,   172,   430,   172,   172,   430,   172,
     172,   430,   430,   170,  1766,   170,   430,   244,   172,   172,
     430,   430,   248,   172,   431,   172,   172,   430,  1780,   431,
     430,   432,   430,   167,   134,   172,   172,   172,   374,   432,
     373,   170,   430,   430,   172,   172,   170,   430,   430,   430,
     430,   430,   430,   172,   170,   170,   170,   430,   170,   172,
     430,   172,    64,   430,   430,   430,    64,   430,   430,   430,
     171,   170,   430,   430,   170,   170,   430,    34,   172,   172,
     430,   430,   430,   172,   172,   170,   172,   170,   172,   430,
     430,   172,   430,   172,   172,   172,   172,   172,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     156,    34,    34,    34,   172,   202,   170,   170,   170,    37,
     212,   171,   426,   170,   430,   419,   167,   172,   430,   430,
     430,   430,   430,   430,   430,   172,   172,   172,   172,   172,
     172,   172,   172,   430,   172,   430,   430,   369,   172,   172,
     172,   172,   369,   420,   172,   172,   430,   430,   430,    34,
     172,   172,   430,   172,   430,    37,   430,   430,    34,   170,
     430,   226,   172,   172,   430,   430,   430,   430,   430,   172,
     207,   411,   430,   430,   430,   430,   172,   172,   430,   172,
     172,   430,   430,   170,   430,   170,   430,   430,   430,   430,
     204,   172,   430,   251,   172,   430,   430,   251,   172,   430,
     430,   170,   172,   109,   172,   235,   235,   369,   369,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   170,   170,
     226,   138,   172,   172,   172,   172,   172,   138,   138,   138,
     172,   213,   172,   172,   172,   430,   172,   172,   172,   353,
     134,   369,   172,   172,   167,   172,    13,   430,   430,   401,
     172,   172,    13,   430,   172,   415,   430,   172,   172,   430,
     430,   430,   430,   136,   430,   172,   167,   172,   172,   172,
     172,   134,   172,   172,   432,   430,   430,   430,   386,   430,
     430,   430,   430,   228,   172,   172,   172,   430,   430,   430,
     159,   172,   432,   409,   172,   430,   432,   430,   430,   430,
     172,   414,   432,   430,   172,   172,   172,   172,   172,   134,
     430,   430,   430,   430,   172,   430,   172,   369,   172,   172,
     410,   172,   138,   430,   138,   172,   172,   172,    64,   430,
     430,    64,   252,   417,   247,   172,   172,   235,   430,   172,
     172,   172,   172,   167,   228,   172,   172,   430,   430,   170,
     430,   430,   172,   172,   172,   172,   369,   172,   167,   172,
     172,   172,   401,   170,   172,   369,   172,   172,   172,   430,
     172,   172,   170,   172,   170,   167,   134,   430,   172,   172,
     430,    25,   172,   124,   172,   172,   172,   172,   172,   172,
     374,   172,   170,   172,   430,   421,   134,   172,   172,   172,
     172,   172,   172,   172,   172,   430,   430,   430,   349,   172,
     430,   170,   430,   430,   430,   172,   172,   172,   172,   172,
     172,  1772,  1922,   430,  1688,   430,   261,  1765,   430,   430,
    1322,  1325,  1918,  1893,   289,   511,   741,  1195,  1092,   488,
    1423,  1044,  1197,   907,  1781,  1903,  1229,  1728,  1493,   150,
     753,  1520,  1666,  1671,    -1,   958,  1668,    -1,    -1,   958,
    1037,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     830,    -1,    -1,    -1,    -1,    -1,   835,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   629,    -1,    -1,   634,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   669,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   692
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,   438,   443,     0,     8,    32,    61,    64,    68,
      75,    77,    81,   130,   134,   157,   162,   163,   208,   209,
     210,   231,   238,   239,   240,   242,   243,   244,   245,   246,
     248,   253,   254,   255,   260,   270,   272,   273,   300,   301,
     302,   314,   316,   319,   354,   439,   441,   442,   445,   446,
     447,   448,   449,   450,   454,   455,   458,   459,   553,   555,
     557,   558,   585,   587,   588,   606,   607,   612,   613,   620,
     621,   622,   643,   644,   660,   662,   774,   776,   794,   796,
     798,   809,   846,   847,   848,   849,   850,   862,   874,   875,
     876,   877,   878,   879,   880,   881,   777,   172,   460,   663,
     172,    93,    94,   623,   645,   586,   799,   800,   801,   810,
     171,   440,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   171,    93,    94,   795,   797,   172,   172,   172,   172,
     317,   318,   451,    84,   100,   452,   172,   456,   463,   170,
     556,    50,    64,   591,   597,   600,   609,   615,   648,   666,
     780,    37,   444,   170,   430,   170,   170,   172,   430,   430,
     170,   170,   170,   170,   170,   170,   811,   430,   170,   172,
     430,   430,   430,   430,   851,   430,   430,   430,   430,   430,
     430,   170,   170,   430,   430,   430,   430,   430,    93,    94,
     453,   235,   430,    14,    26,    30,    37,   106,   110,   147,
     165,   295,   457,     9,    14,    15,    33,    35,    37,    53,
      86,   101,   110,   128,   134,   143,   155,   167,   236,   256,
     258,   271,   290,   291,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   347,   349,
     357,   358,   359,   366,   367,   368,   378,   379,   380,   381,
     390,   393,   402,   403,   404,   405,   406,   461,   464,    29,
     382,    48,    64,   110,   153,   163,   236,   559,   566,   567,
     569,   573,   574,   577,   578,    29,   590,   601,   597,   598,
     602,    37,   125,   608,   610,   611,    37,   142,   614,   616,
     619,    16,    37,   131,   141,   395,   646,   649,   650,   651,
     654,    16,    18,    36,    50,    51,    66,    84,    88,   100,
     106,   130,   131,   132,   141,   148,   236,   266,   375,   574,
     661,   667,   670,   674,   679,   680,   681,   682,   683,   684,
     685,   687,   689,   690,   692,   693,   694,   742,   743,   744,
     751,   753,   754,   130,   197,   198,   199,   200,   201,   202,
     691,   775,   781,   787,   798,    67,   430,   624,   433,   433,
     433,    37,    58,    64,    67,    68,    81,   100,   162,   163,
     812,   430,   430,   863,   430,   433,   433,   430,   554,    99,
      73,   388,   157,    74,    85,    78,   166,   297,   172,    25,
     172,    54,   160,   376,   377,   541,   172,   462,   172,   172,
     172,   124,   172,   172,   172,   172,    28,    92,   122,   190,
     237,   352,   540,   172,   469,   172,   257,   431,   172,   172,
     292,   293,   294,   521,   292,   172,   475,   172,   476,   172,
     172,   478,   172,   479,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   481,
     370,   371,   505,   172,   172,   172,   172,   172,   416,   487,
     172,   172,   172,   172,   490,   505,   465,   431,   172,   430,
     172,   575,   579,   172,   560,   570,    37,   583,   568,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   430,   431,   576,   659,
     580,   589,   170,   170,   236,   557,   592,   593,   594,   599,
      33,    72,    91,   109,   110,   134,   167,   349,   603,   134,
     170,    61,   170,   617,    22,    96,   237,   647,   172,   652,
     655,    10,    22,    23,    24,    38,    96,   112,   237,   323,
     348,   675,   752,   686,   170,   170,   688,   659,   695,   172,
     172,    10,    50,   189,   671,   668,   267,   268,    64,   746,
     430,   576,   659,    37,   664,   170,   733,   698,    37,    64,
      97,   103,   109,   162,   167,   719,   720,   729,   756,   783,
     784,   785,   170,   786,   172,   782,    37,   778,   788,   384,
     627,   172,   211,   425,   431,   805,   171,   211,   431,   807,
     211,   214,   215,   224,   431,   805,   806,   807,   231,   814,
     819,   813,   816,   820,   815,   817,   818,   249,   250,   865,
     866,   867,   249,   250,   853,   854,   855,   807,   807,   170,
     172,   172,   172,   172,   172,   172,   172,   172,   430,   172,
     257,   430,   430,   430,   170,   430,   172,   430,   172,   430,
     172,   257,   430,   430,   467,   430,   430,   430,   170,   532,
     533,   430,   431,   172,   430,   430,   172,   470,   167,   172,
     296,   430,   172,   257,   516,   430,   516,   477,   430,   516,
     430,   516,   480,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   172,   430,   430,   167,   361,   362,   363,
     364,   543,   172,   430,   484,   486,   430,   353,   369,    21,
     365,   422,   495,   430,   430,   172,   430,   430,   659,   726,
     172,   412,   491,   659,   430,   430,   170,   170,   430,   170,
     170,   571,   572,   584,   569,   172,   172,   430,   430,   576,
     103,   109,   581,   591,   430,   170,   171,   172,   595,   170,
      37,   604,   594,    54,   160,   172,   172,   659,   172,   172,
     172,   172,   170,   172,   430,   618,   430,   430,   430,   170,
      13,   168,   169,   182,   430,   653,   656,   356,   396,    46,
     133,   144,   145,   315,   400,   677,   348,    11,    12,   105,
     107,   151,   152,   678,    55,    56,    89,   106,   133,   355,
     676,   430,   170,   170,   170,   430,   170,   430,   170,   430,
      13,   430,   430,   430,   168,   169,   182,   430,   672,   170,
     669,   673,   430,   430,   747,   745,   430,   430,   576,   665,
     172,   430,   430,    14,    27,    33,    37,    44,    45,    57,
      63,    65,    66,    70,    71,    76,    90,   104,   106,   108,
     110,   116,   117,   120,   121,   126,   127,   146,   158,   161,
     164,   236,   250,   259,   261,   262,   263,   264,   265,   324,
     325,   326,   327,   328,   329,   330,   331,   347,   383,   391,
     397,   574,   696,   699,   713,   714,   721,    62,   726,    62,
     726,    62,   659,    62,   659,   172,    37,   723,    37,    39,
      40,    41,    42,    43,    49,   113,   114,   115,   118,   119,
     150,   156,   205,   206,   207,   227,   228,   230,   279,   280,
     284,   285,   286,   287,   288,   289,   755,   757,   761,   762,
     770,   170,   733,   733,   168,   169,   734,   168,   169,   741,
     792,   733,   779,    37,   197,   198,   789,   430,   628,   806,
     805,   805,   425,   426,   427,   428,   430,   434,   802,   806,
     807,   426,   802,   806,   806,   805,   806,   807,   216,   217,
     218,   219,   220,   221,   222,   223,   433,   435,   436,   808,
     216,   217,   222,   223,   802,   222,   223,   808,   170,   170,
     170,   170,   170,   170,   170,   170,   172,   868,    37,   864,
     866,   172,   856,    37,   852,   854,   802,   802,   170,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   431,   430,
     430,   430,   431,   125,   372,   418,   831,   832,   170,   171,
     172,   430,   533,   431,   534,   535,   172,   430,   295,   522,
     172,   430,   172,   431,   430,   430,   313,   520,   430,   430,
     520,   430,   172,   430,   172,   167,   542,   513,   172,   172,
     167,   172,   167,   430,   430,   728,   172,   167,   492,   659,
     430,   430,   170,   171,   172,   430,   572,   170,   172,   430,
     582,   659,   592,   594,   596,   430,   605,   430,   430,   430,
     430,   659,   430,    13,   149,   172,   172,   172,   172,   170,
     430,   657,   430,   430,   430,   430,   172,   170,   171,   172,
     430,   673,   170,    37,   746,   430,   170,   172,   172,     6,
     111,    46,    55,    56,    89,   697,   172,   172,   704,   170,
     172,   700,   172,   172,   701,   702,   172,   172,   172,   172,
     172,   172,   172,    56,    89,   137,   139,   717,     5,    46,
     112,   718,   172,     7,    17,    31,    52,   106,   129,   716,
     172,   172,   705,   703,   170,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   706,   709,
     707,   708,   140,   430,   659,    16,    37,   715,   170,   726,
     728,   726,   659,   727,   659,   659,   659,   730,   430,   720,
     148,   172,   172,   172,   172,   758,   172,   172,   172,   172,
     759,    60,    82,    83,   771,   206,   172,   768,   431,   766,
     767,   430,   171,   171,   171,   226,   281,   282,   283,   763,
      59,   225,   229,   765,   430,   430,   172,   172,   430,   172,
     172,   430,    37,   203,   793,   430,   170,   170,   790,   791,
      64,   170,   236,   389,   398,   399,   555,   606,   625,   629,
     630,   631,   632,   633,   636,   212,   434,   803,   432,   805,
     805,   805,   805,   803,   432,   807,   803,   432,   805,   805,
     805,   806,   806,   806,   806,   807,   807,   807,   232,   233,
     234,   269,   821,   821,   821,   821,   821,   821,   821,   821,
     430,   172,   869,   244,   802,   430,   172,   857,   248,   802,
     430,   431,   536,   537,   431,   538,   539,   833,     9,    64,
     235,   299,   346,   407,   408,   414,   834,   831,   172,   432,
     535,   432,   172,   473,   471,   659,   430,   430,   485,   488,
     172,   430,   138,   385,   387,   392,   413,   430,   514,   515,
     167,   134,   172,   496,   172,   430,   727,   489,   172,   374,
     482,   373,   432,   726,   659,   604,   430,   594,   170,   430,
     172,   172,   430,   136,   430,   430,   576,   430,   430,   576,
     430,   430,   430,   430,   430,   430,   154,   430,   170,   430,
     430,   172,   170,   430,   170,   430,   430,   170,   172,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   170,   711,   712,   172,   430,    64,   842,   842,
     842,   430,   430,   842,   842,   842,   842,    64,   844,   844,
     842,   844,   361,   362,   363,   364,   710,   170,   171,   170,
     430,   659,   430,   576,   675,   719,   722,   728,   430,   727,
     727,   659,   430,   731,   170,   172,   172,   172,   172,   170,
     772,   172,   172,   172,   172,   170,   773,   430,   172,   172,
     430,   172,   430,   767,   430,   430,   430,   172,   281,   282,
     283,   764,   172,   156,    34,    34,    34,    34,   202,   172,
     733,   733,   637,   170,   171,   172,   634,   170,   170,   170,
      37,   626,   212,   805,   807,   805,   806,   807,   171,   822,
     823,   170,   430,   430,   430,   430,   430,   430,   430,   430,
     172,   430,   172,   430,   172,   432,   537,   172,   432,   539,
     419,   828,   172,   835,   172,   172,   172,   172,   172,   172,
     430,   172,   430,   430,   523,   531,   531,   659,   369,   510,
     167,   353,   506,   172,   172,   172,   172,   369,   531,   420,
     550,   167,   509,   172,   518,   172,   727,   430,   430,   430,
     430,   658,   748,    34,   430,   172,   430,   430,   430,   172,
     170,   171,   172,   430,   712,   172,   843,   430,   430,   430,
     430,   430,   430,   430,   845,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   576,   430,   430,    37,   411,   724,
      34,   732,   727,   728,   732,   170,   430,   430,   430,   430,
     430,   170,   430,   430,   430,   430,   430,   170,   430,   207,
     172,   172,   226,   172,   771,   172,   172,   172,   172,   204,
     430,   430,   170,   633,   635,   172,   430,   430,   170,   213,
     434,   804,   804,   804,   235,   829,   829,   251,   870,   871,
     251,   858,   859,   172,   430,   172,   430,   170,   172,   235,
     369,   369,   415,   432,   531,   172,   430,   430,   517,   172,
     350,   351,   511,   172,   172,   430,   430,   468,   172,   497,
     172,   172,   172,   430,   401,   432,   519,   659,   172,   727,
     109,   750,   172,   430,   430,   430,   170,   170,   430,    20,
     134,   725,   172,   430,   728,   430,   430,   732,   172,   172,
     172,   172,   760,   226,   138,   138,   138,   138,   172,   638,
     430,   633,   430,   213,   805,   807,   806,   172,   172,   830,
     830,   172,   871,   172,   859,   432,   432,   836,   838,   172,
     172,   172,   172,   430,   474,   472,   518,   353,   512,   507,
     369,   503,   504,   134,   167,   500,   172,   430,   430,   728,
     659,   749,    13,   172,   172,   430,    13,   732,   430,   770,
     432,   430,   172,   172,   769,   172,   172,   172,   172,   172,
     430,   167,   172,   430,   430,   136,   840,    21,   235,   298,
     824,   172,   837,   839,   414,   524,   228,   529,   529,   432,
     172,   430,   409,   508,   172,   430,   503,   172,   172,   531,
     134,   483,   386,   430,   659,   750,   172,   172,   430,   430,
     172,   172,   159,   172,   735,   737,   172,   872,   860,   172,
     172,   410,   841,   417,   825,   172,   167,   296,   526,   172,
     369,   172,   134,   502,   498,   501,   172,   561,   172,   138,
     138,   430,   172,   172,   172,    64,    64,   430,   252,   247,
     235,   172,   172,   172,   172,   228,   530,   172,   172,   551,
     531,   167,   547,   548,   466,   170,   430,   172,   172,   172,
     172,   430,   736,   738,   639,   172,   873,   172,   861,   172,
     369,   528,   527,   172,   531,   167,   552,   499,   172,   547,
     401,   493,   494,   170,   172,   172,   172,   430,   739,   739,
     641,   172,   430,   172,   430,   172,   172,   531,   531,   525,
     430,   172,   544,   550,   172,   430,   493,   170,   172,   170,
     740,    14,    35,    37,   110,   134,   271,   381,   642,   826,
     430,   430,   531,   369,   167,   545,   172,   134,   430,   172,
      25,   172,   640,   124,   172,   172,   172,   421,   827,   430,
     172,   172,   549,   172,   374,   172,   430,   170,   172,   430,
     430,   430,   134,   546,   531,   172,   430,   430,   172,   531,
     172,   430,   349,   172,   172,   172,   172,   430,   562,   563,
      86,    88,    98,   394,   564,   172,   172,   565,   172,   172,
     172,   170,   172,   172,   430,   430,   430,   172,   430
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 597 "lef.y"
    {
 /* 11/16/2001 - Wanda da Rosa - pcr 408334
 */
/* Return 1 if there are errors
 */
if (lef_errors)
 return 1;
 if (!hasVer && versionNum < 5.6)
 lefWarning(2001, "VERSION is a required statement on LEF file with version 5.5 and earlier.\nWithout VERSION defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Reference manual on how to defined this statement.");
 /* pre 5.6, 5.6 it is obsolete
 */
if (!hasNameCase && versionNum < 5.6)
 lefWarning(2002, "NAMESCASESENSITIVE is a required statement on LEF file with version 5.5 and earlier.\nWithout NAMESCASESENSITIVE defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
 if (!hasBusBit && versionNum < 5.6)
 lefWarning(2003, "BUSBITCHARS is a required statement on LEF file with version 5.5 and earlier.\nWithout BUSBITCHARS defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
 if (!hasDivChar && versionNum < 5.6)
 lefWarning(2004, "DIVIDERCHAR is a required statementon LEF file with version 5.5 and earlier.\nWithout DIVIDECHAR defined, the LEF file is technically illegal.\nRefer the LEF/DEF 5.5 and earlier Language Referece manual on how to define this statement.");
 
 (void)resetVars();
 /* 
hasVer = 0;
 hasNameCase = 0;
 hasBusBit = 0;
 hasDivChar = 0;
 hasManufactur = 0;
 hasMinfeature = 0;
 hasSite = 0;
 antennaInoutWarnings = 0;
 antennaInputWarnings = 0;
 antennaOutputWarnings = 0;
 arrayWarnings = 0;
 caseSensitiveWarnings = 0;
 correctionTableWarnings = 0;
 dielectricWarnings = 0;
 edgeRateThreshold1Warnings = 0;
 edgeRateThreshold2Warnings = 0;
 edgeRateScaleFactorWarnings = 0;
 inoutAntennaWarnings = 0;
 inputAntennaWarnings = 0;
 iRDropWarnings = 0;
 layerWarnings = 0;
 macroWarnings = 0;
 maxStackViaWarnings = 0;
 minFeatureWarnings = 0;
 noiseMarginWarnings = 0;
 noiseTableWarnings = 0;
 nonDefaultWarnings = 0;
 noWireExtensionWarnings = 0;
 outputAntennaWarnings = 0;
 siteWarnings = 0;
 spacingWarnings = 0;
 timingWarnings = 0;
 unitsWarnings = 0;
 useMinSpacingWarnings = 0;
 viaRuleWarnings = 0;
 viaWarnings = 0;
 */

 ;}
    break;

  case 3:
#line 657 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 4:
#line 658 "lef.y"
    {
 versionNum = convert_name2num((yyvsp[(3) - (4)].string));
 if (versionNum > 5.7) {
 char temp[120];
 sprintf(temp,
 "Lef parser 5.7 does not support lef file with version %s. Parser stops executions.", (yyvsp[(3) - (4)].string));
 lefError(1503, temp);
 return 1;
 }
 /* 
versionNum = $3; Save the version number for future use */

 if (lefrVersionStrCbk) {
 CALLBACK(lefrVersionStrCbk, lefrVersionStrCbkType, (yyvsp[(3) - (4)].string));
 } else {
 if (lefrVersionCbk)
 CALLBACK(lefrVersionCbk, lefrVersionCbkType, versionNum);
 }
 if (versionNum > 5.3 && versionNum < 5.4) {
 ignoreVersion = 1;
 }
 use5_3 = use5_4 = 0;
 lef_errors = 0;
 if (hasVer) /* More than 1 lef file within the open file */

 (void)resetVars();
 hasVer = 1;
 if (versionNum < 5.6) {
 doneLib = 0;
 lefNamesCaseSensitive = lefReaderCaseSensitive;
 } else {
 doneLib = 1;
 lefNamesCaseSensitive = 1;
 }
 ;}
    break;

  case 5:
#line 696 "lef.y"
    {
 if (lefrDividerCharCbk) {
 if (strcmp((yyvsp[(2) - (3)].string), "") != 0) {
 CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, (yyvsp[(2) - (3)].string));
 } else {
 CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, "/");
 lefWarning(2005, "DIVIDERCHAR has an invalid null value. Value is set to default /");
 }
 }
 hasDivChar = 1;
 ;}
    break;

  case 6:
#line 709 "lef.y"
    {
 if (lefrBusBitCharsCbk) {
 if (strcmp((yyvsp[(2) - (3)].string), "") != 0) {
 CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, (yyvsp[(2) - (3)].string));
 } else {
 CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, "[]");
 lefWarning(2006, "BUSBITCHAR has an invalid null value. Value is set to default []");
 }
 }
 hasBusBit = 1;
 ;}
    break;

  case 9:
#line 725 "lef.y"
    { ;}
    break;

  case 10:
#line 729 "lef.y"
    {
 if (versionNum >= 5.6) {
 doneLib = 1;
 ge56done = 1;
 }
 ;}
    break;

  case 11:
#line 736 "lef.y"
    {
 doneLib = 1;
 ge56done = 1;
 if (lefrLibraryEndCbk)
 CALLBACK(lefrLibraryEndCbk, lefrLibraryEndCbkType, 0);
 /* 11/16/2001 - Wanda da Rosa - pcr 408334
 */
/* Return 1 if there are errors
 */
/* 
if (lef_errors)
 return 1;
 if (!hasVer)
 lefWarning(2001, "VERSION is a required statement.");
 if (!hasNameCase)
 lefWarning(2002, "NAMESCASESENSITIVE is a required statement.");
 if (!hasBusBit && versionNum < 5.6)
 lefWarning(2003, "BUSBITCHARS is a required statement.");
 if (!hasDivChar && versionNum < 5.6)
 lefWarning(2004, "DIVIDERCHAR is a required statement.");
 hasVer = 0;
 hasNameCase = 0;
 hasBusBit = 0;
 hasDivChar = 0;
 hasManufactur = 0;
 hasMinfeature = 0;
 hasSite = 0;
 */

 ;}
    break;

  case 50:
#line 782 "lef.y"
    {
 if (versionNum < 5.6) {
 lefNamesCaseSensitive = TRUE;
 if (lefrCaseSensitiveCbk)
 CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
 			 lefNamesCaseSensitive);
 hasNameCase = 1;
 } else
 if (lefrCaseSensitiveCbk) /* write warning only if cbk is set */

 if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings)
 lefWarning(2007, "NAMESCASESENSITIVE statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 	 ;}
    break;

  case 51:
#line 796 "lef.y"
    {
 if (versionNum < 5.6) {
 	 lefNamesCaseSensitive = FALSE;
 if (lefrCaseSensitiveCbk)
 	 CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
 			 lefNamesCaseSensitive);
 hasNameCase = 1;
 } else {
 if (lefrCaseSensitiveCbk) { /* write error only if cbk is set */

 if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings) {
 lefError(1504, "NAMESCASESENSITIVE statement is set with OFF.\nStarting version 5.6, NAMESCASENSITIVE is obsolete,\nif it is defined, it has to have the ON value.\nParser stops executions.");
 CHKERR();
 }
 }
 }
 	 ;}
    break;

  case 52:
#line 815 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrNoWireExtensionCbk)
 CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "ON");
 } else
 if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */

 if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
 lefWarning(2008, "NOWIREEXTENSIONATPIN statement is obsolete in version 5.6 or later.\nThe NOWIREEXTENSIONATPIN statement will be ignored.");
 ;}
    break;

  case 53:
#line 826 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrNoWireExtensionCbk)
 CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "OFF");
 } else
 if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */

 if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
 lefWarning(2008, "NOWIREEXTENSIONATPIN statement is obsolete in version 5.6 or later.\nThe NOWIREEXTENSIONATPIN statement will be ignored.");
 ;}
    break;

  case 54:
#line 838 "lef.y"
    {
 if (lefrManufacturingCbk)
 CALLBACK(lefrManufacturingCbk, lefrManufacturingCbkType, (yyvsp[(2) - (3)].dval));
 hasManufactur = 1;
 ;}
    break;

  case 55:
#line 845 "lef.y"
    {
 if ((strcmp((yyvsp[(2) - (4)].string), "PIN") == 0) && (versionNum >= 5.6)) {
 if (lefrUseMinSpacingCbk) /* write warning only if cbk is set */

 if (useMinSpacingWarnings++ < lefrUseMinSpacingWarnings)
 lefWarning(2009, "USEMINSPACING PIN statement is obsolete in version 5.6 or later.\n The USEMINSPACING PIN statement will be ignored.");
 } else {
 if (lefrUseMinSpacingCbk) {
 lefiUseMinSpacing_set( &(lefrUseMinSpacing),
 (yyvsp[(2) - (4)].string),
(yyvsp[(3) - (4)].integer)) ;
 CALLBACK(lefrUseMinSpacingCbk, lefrUseMinSpacingCbkType,
 &lefrUseMinSpacing);
 }
 }
 ;}
    break;

  case 56:
#line 863 "lef.y"
    { CALLBACK(lefrClearanceMeasureCbk, lefrClearanceMeasureCbkType, (yyvsp[(2) - (3)].string)); ;}
    break;

  case 57:
#line 866 "lef.y"
    {(yyval.string) = (char*)"MAXXY";;}
    break;

  case 58:
#line 867 "lef.y"
    {(yyval.string) = (char*)"EUCLIDEAN";;}
    break;

  case 59:
#line 870 "lef.y"
    {(yyval.string) = (char*)"OBS";;}
    break;

  case 60:
#line 871 "lef.y"
    {(yyval.string) = (char*)"PIN";;}
    break;

  case 61:
#line 874 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 62:
#line 875 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 63:
#line 878 "lef.y"
    {
 if (lefrUnitsCbk)
 CALLBACK(lefrUnitsCbk, lefrUnitsCbkType, &lefrUnits);
 ;}
    break;

  case 64:
#line 884 "lef.y"
    {
 lefiUnits_clear( &(lefrUnits) ) ;
 if (hasManufactur) {
 if (unitsWarnings++ < lefrUnitsWarnings) {
 lefError(1505, "MANUFACTURINGGRID statement was defined before UNITS.\nRefer the LEF Language Reference manual for the order of LEF statements.");
 CHKERR();
 }
 }
 if (hasMinfeature) {
 if (unitsWarnings++ < lefrUnitsWarnings) {
 lefError(1712, "MINFEATURE statement was defined before UNITS.\nRefer the LEF Language Reference manual for the order of LEF statements.");
 CHKERR();
 }
 }
 if (versionNum < 5.6) {
 if (hasSite) {/*SITE is defined before UNIT and is illegal in pre 5.6*/
 lefError(1713, "SITE statement was defined before UNITS.\nRefer the LEF Language Reference manual for the order of LEF statements.");
 CHKERR();
 }
 }
 ;}
    break;

  case 67:
#line 912 "lef.y"
    { if (lefrUnitsCbk) lefiUnits_setTime( &(lefrUnits),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 68:
#line 916 "lef.y"
    { if (lefrUnitsCbk) lefiUnits_setCapacitance( &(lefrUnits),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 69:
#line 920 "lef.y"
    { if (lefrUnitsCbk) lefiUnits_setResistance( &(lefrUnits),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 70:
#line 924 "lef.y"
    { if (lefrUnitsCbk) lefiUnits_setPower( &(lefrUnits),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 71:
#line 928 "lef.y"
    { if (lefrUnitsCbk) lefiUnits_setCurrent( &(lefrUnits),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 72:
#line 932 "lef.y"
    { if (lefrUnitsCbk) lefiUnits_setVoltage( &(lefrUnits),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 73:
#line 936 "lef.y"
    {
 if(validNum((int)(yyvsp[(3) - (4)].dval))) {
 if (lefrUnitsCbk)
 lefiUnits_setDatabase( &(lefrUnits),
 "MICRONS",
(yyvsp[(3) - (4)].dval)) ;
 }
 ;}
    break;

  case 74:
#line 945 "lef.y"
    { if (lefrUnitsCbk) lefiUnits_setFrequency( &(lefrUnits),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 75:
#line 951 "lef.y"
    {
 if (lefrLayerCbk)
 CALLBACK(lefrLayerCbk, lefrLayerCbkType, &lefrLayer);
 ;}
    break;

  case 76:
#line 956 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 77:
#line 957 "lef.y"
    {
 if (lefrHasMaxVS) { /* 5.5 */

 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1506, "A MAXVIASTACK statement is defined before the LAYER statement.\nRefer to the LEF Language Reference manual for the order of LEF statements.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setName( &(lefrLayer),
 (yyvsp[(3) - (3)].string)) ;
 useLenThr = 0;
 layerCut = 0;
 layerMastOver = 0;
 layerRout = 0;
 layerDir = 0;
 lefrHasLayer = 1;
 /* $3);
  */
layerName = strdup((yyvsp[(3) - (3)].string));
 hasType = 0;
 hasPitch = 0;
 hasWidth = 0;
 hasDirection = 0;
 hasParallel = 0;
 hasInfluence = 0;
 hasTwoWidths = 0;
 lefrHasSpacingTbl = 0;
 lefrHasSpacing = 0;
 ;}
    break;

  case 78:
#line 991 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 79:
#line 992 "lef.y"
    {
 if (strcmp(layerName, (yyvsp[(3) - (3)].string)) != 0) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END LAYER name %s is different from the LAYER name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (3)].string), layerName);
 lefError(1507, outMsg);
 lefFree(outMsg);
 lefFree(layerName);
 CHKERR();
 } else
 lefFree(layerName);
 } else
 lefFree(layerName);
 } else
 lefFree(layerName);
 if (!lefrRelaxMode) {
 if (hasType == 0) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1508, "TYPE statement is a required statement in a LAYER and it is not defined.");
 CHKERR();
 }
 }
 }
 if ((layerRout == 1) && (hasPitch == 0)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1509, "PITCH statement is a required statement in a LAYER with type ROUTING and it is not defined.");
 CHKERR();
 }
 }
 }
 if ((layerRout == 1) && (hasWidth == 0)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1510, "WIDTH statement is a required statement in a LAYER with type ROUTING and it is not defined.");
 CHKERR();
 }
 }
 }
 if ((layerRout == 1) && (hasDirection == 0)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg, "The DIRECTION statement which is required in a LAYER with TYPE ROUTING is not defined in LAYER %s.\nUpdate your lef file and add the DIRECTION statement for layer %s.", (yyvsp[(3) - (3)].string), (yyvsp[(3) - (3)].string));
 lefError(1511, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 ;}
    break;

  case 80:
#line 1056 "lef.y"
    { ;}
    break;

  case 81:
#line 1063 "lef.y"
    { ;}
    break;

  case 82:
#line 1068 "lef.y"
    {
 /* let setArraySpacingCutSpacing to set the data */

 ;}
    break;

  case 83:
#line 1075 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_setArraySpacingCut( &(lefrLayer),
 (yyvsp[(6) - (10)].dval)) ;
 lefiLayer_addArraySpacingArray( &(lefrLayer),
 (int)(yyvsp[(8) - (10)].dval),(yyvsp[(10) - (10)].dval)) ;
 arrayCutsVal = (int)(yyvsp[(8) - (10)].dval); /* set the value */

 arrayCutsWar = 0;
 }
 ;}
    break;

  case 84:
#line 1087 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "ARRAYSPACING is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1685, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 ;}
    break;

  case 85:
#line 1098 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setType( &(lefrLayer),
 (yyvsp[(2) - (3)].string)) ;
 hasType = 1;
 ;}
    break;

  case 86:
#line 1105 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setPitch( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 hasPitch = 1;
 ;}
    break;

  case 87:
#line 1111 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setPitchXY( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 hasPitch = 1;
 ;}
    break;

  case 88:
#line 1118 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setDiagPitch( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 89:
#line 1123 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setDiagPitchXY( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 90:
#line 1129 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setOffset( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 91:
#line 1134 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setOffsetXY( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 92:
#line 1140 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setDiagWidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 93:
#line 1145 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setDiagSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 94:
#line 1151 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setWidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 hasWidth = 1;
 ;}
    break;

  case 95:
#line 1157 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_setArea( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 96:
#line 1162 "lef.y"
    {
 hasSpCenter = 0; /* reset to 0, only once per spacing is allowed */

 hasSpSamenet = 0;
 hasSpParallel = 0;
 hasSpLayer = 0;
 layerCutSpacing = (yyvsp[(2) - (2)].dval); /* for error message purpose */

 /* 11/22/99 - Wanda da Rosa, PCR 283762
 */
/*            Issue an error is this is defined in masterslice
 */
if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1512, "It is illegal to define a SPACING statement in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
 CHKERR();
 }
 }
 }
 /* 5.5 either SPACING or SPACINGTABLE, not both for routing layer only
 */
if (layerRout) {
 if (lefrHasSpacingTbl && versionNum < 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefWarning(2010, "It is illegal to have both SPACING rules & SPACINGTABLE rules within a ROUTING layer");
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setSpacingMin( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 lefrHasSpacing = 1;
 } else {
 if (lefrLayerCbk)
 lefiLayer_setSpacingMin( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 ;}
    break;

  case 98:
#line 1208 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingTableOrtho( &(lefrLayer) ) ;
 if (lefrLayerCbk) /* due to converting to C, else, convertor produce */

 /* badlefiLayer_addSpacingTableOrthoWithin( &(lefrLayer),
 $4,
$6) ;
 code*/

 ;}
    break;

  case 99:
#line 1220 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "SPACINGTABLE ORTHOGONAL is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1694, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 ;}
    break;

  case 100:
#line 1231 "lef.y"
    {
 layerDir = 1;
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1513, "DIRECTION statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setDirection( &(lefrLayer),
 (yyvsp[(2) - (3)].string)) ;
 hasDirection = 1;
 ;}
    break;

  case 101:
#line 1247 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1514, "RESISTANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setResistance( &(lefrLayer),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 102:
#line 1261 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1515, "RESISTANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 ;}
    break;

  case 103:
#line 1273 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1516, "CAPACITANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCapacitance( &(lefrLayer),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 104:
#line 1287 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1517, "CAPACITANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 ;}
    break;

  case 105:
#line 1299 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1518, "HEIGHT statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setHeight( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 106:
#line 1313 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1519, "WIREEXTENSION statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setWireExtension( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 107:
#line 1327 "lef.y"
    {
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1520, "THICKNESS statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setThickness( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 108:
#line 1341 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1521, "SHRINKAGE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setShrinkage( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 109:
#line 1355 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1522, "CAPMULTIPLIER statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCapMultiplier( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 110:
#line 1369 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1523, "EDGECAPACITANCE statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setEdgeCap( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 111:
#line 1398 "lef.y"
    { /* 5.3 syntax */

 use5_3 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1525, "ANTENNALENGTHFACTOR statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNALENGTHFACTOR statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNALENGTHFACTOR syntax, which is illegal.", versionNum) ;
 lefError(1526, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 
 if (lefrLayerCbk) lefiLayer_setAntennaLength( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 112:
#line 1429 "lef.y"
    {
 if (versionNum < 5.2) {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1526, "CURRENTDEN statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCurrentDensity( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else {
 if (layerWarnings++ < lefrLayerWarnings) {
 lefWarning(2079, "CURRENTDEN statement is obsolete in version 5.2 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.2 or later.");
 CHKERR();
 }
 }
 ;}
    break;

  case 113:
#line 1450 "lef.y"
    {
 if (versionNum < 5.2) {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1526, "CURRENTDEN statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 } else {
 if (layerWarnings++ < lefrLayerWarnings) {
 lefWarning(2079, "CURRENTDEN statement is obsolete in version 5.2 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.2 or later.");
 CHKERR();
 }
 }
 ;}
    break;

  case 114:
#line 1469 "lef.y"
    {
 if (versionNum < 5.2) {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1526, "CURRENTDEN statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCurrentPoint( &(lefrLayer),
 (yyvsp[(3) - (6)].dval),
(yyvsp[(4) - (6)].dval)) ;
 } else {
 if (layerWarnings++ < lefrLayerWarnings) {
 lefWarning(2079, "CURRENTDEN statement is obsolete in version 5.2 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.2 or later.");
 CHKERR();
 }
 }
 ;}
    break;

  case 115:
#line 1490 "lef.y"
    { lefDumbMode = 10000000; lefRealNum = 1; lefInProp = 1; ;}
    break;

  case 116:
#line 1491 "lef.y"
    {
 lefDumbMode = 0;
 lefRealNum = 0;
 lefInProp = 0;
 ;}
    break;

  case 117:
#line 1497 "lef.y"
    {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1527, "ACCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addAccurrentDensity( &(lefrLayer),
 (yyvsp[(2) - (2)].string)) ;
 ;}
    break;

  case 119:
#line 1512 "lef.y"
    {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1527, "ACCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addAccurrentDensity( &(lefrLayer),
 (yyvsp[(2) - (4)].string)) ;
 lefiLayer_setAcOneEntry( &(lefrLayer),
 (yyvsp[(3) - (4)].dval)) ;
 }
 ;}
    break;

  case 120:
#line 1530 "lef.y"
    {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1528, "DCCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 lefiLayer_setDcOneEntry( &(lefrLayer),
 (yyvsp[(3) - (4)].dval)) ;
 }
 ;}
    break;

  case 121:
#line 1548 "lef.y"
    {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1528, "DCCURRENTDENSITY statement can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (!layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1529, "CUTAREA statement can only be defined in LAYER with type CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(4) - (4)].dval)) ;
 }
 ;}
    break;

  case 122:
#line 1575 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addDcCutarea( &(lefrLayer) ) ;
 ;}
    break;

  case 124:
#line 1579 "lef.y"
    {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1528, "DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1530, "WIDTH statement can only be defined in LAYER with type ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(4) - (4)].dval)) ;
 }
 ;}
    break;

  case 125:
#line 1606 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addDcWidth( &(lefrLayer) ) ;
 ;}
    break;

  case 127:
#line 1613 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1531, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNAAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1532, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1533, "ANTENNAAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaAreaRatio( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 128:
#line 1657 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNADIFFAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1532, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNADIFFAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNADIFFAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1533, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1534, "ANTENNADIFFAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaDAR;
 ;}
    break;

  case 130:
#line 1701 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1535, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNACUMAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1536, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1537, "ANTENNACUMAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaCumAreaRatio( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 131:
#line 1745 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMDIFFAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1538, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMDIFFAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNACUMDIFFAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1539, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1540, "ANTENNACUMDIFFAREARATIO statement can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaCDAR;
 ;}
    break;

  case 133:
#line 1789 "lef.y"
    { /* both 5.3 & 5.4 syntax */

 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1541, "ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 /* this does not need to check, since syntax is in both 5.3 & 5.4 */

 if (lefrLayerCbk) lefiLayer_setAntennaAreaFactor( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 antennaType = lefiAntennaAF;
 ;}
    break;

  case 135:
#line 1808 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1542, "ANTENNASIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNASIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1543, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNASIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNASIDEAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1544, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaSideAreaRatio( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 136:
#line 1852 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1545, "ANTENNADIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNADIFFSIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1546, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNADIFFSIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNADIFFSIDEAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1547, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaDSAR;
 ;}
    break;

  case 138:
#line 1896 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1548, "ANTENNACUMSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMSIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1549, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMSIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNACUMSIDEAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1550, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaCumSideAreaRatio( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 139:
#line 1940 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1551, "ANTENNACUMDIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMDIFFSIDEAREARATIO statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1552, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACUMDIFFSIDEAREARATIO statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNACUMDIFFSIDEAREARATIO syntax, which is illegal.", versionNum) ;
 lefError(1553, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaCDSAR;
 ;}
    break;

  case 141:
#line 1984 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1554, "ANTENNASIDEAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNASIDEAREAFACTOR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1555, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNASIDEAREAFACTOR statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNASIDEAREAFACTOR syntax, which is illegal.", versionNum) ;
 lefError(1556, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaSideAreaFactor( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 antennaType = lefiAntennaSAF;
 ;}
    break;

  case 143:
#line 2031 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1557, "ANTENNAMODEL can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1558, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMODEL statement is a version 5.4 or earlier syntax.\nYour lef file with version %g, has both old and ANTENNAMODEL syntax, which is illegal.", versionNum) ;
 lefError(1559, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaO;
 ;}
    break;

  case 145:
#line 2076 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "ANTENNACUMROUTINGPLUSCUT is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1686, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (!layerRout && !layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1560, "ANTENNACUMROUTINGPLUSCUT can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaCumRoutingPlusCut( &(lefrLayer) ) ;
 }
 ;}
    break;

  case 146:
#line 2099 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "ANTENNAGATEPLUSDIFF is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1687, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (!layerRout && !layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1561, "ANTENNAGATEPLUSDIFF can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaGatePlusDiff( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 147:
#line 2123 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "ANTENNAAREAMINUSDIFF is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1688, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (!layerRout && !layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1562, "ANTENNAAREAMINUSDIFF can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaAreaMinusDiff( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 148:
#line 2147 "lef.y"
    {
 if (!layerRout && !layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1563, "ANTENNAAREADIFFREDUCEPWL can only be defined in LAYER with type ROUTING or CUT. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */

 lefrAntennaPWLPtr=(lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
 lefiAntennaPWL_Init( lefrAntennaPWLPtr ) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 (yyvsp[(3) - (4)].pt).x,
(yyvsp[(3) - (4)].pt).y) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 (yyvsp[(4) - (4)].pt).x,
(yyvsp[(4) - (4)].pt).y) ;
 }
 ;}
    break;

  case 149:
#line 2170 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setAntennaPWL( &(lefrLayer),
 lefiAntennaADR,
lefrAntennaPWLPtr) ;
 ;}
    break;

  case 150:
#line 2176 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "ANTENNAAREADIFFREDUCEPWL is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1689, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 ;}
    break;

  case 151:
#line 2187 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 if (lefrLayerCbk) lefiLayer_setSlotWireWidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2011, "SLOTWIREWIDTH statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SLOTWIREWIDTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1564, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrLayerCbk) lefiLayer_setSlotWireWidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 152:
#line 2217 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 if (lefrLayerCbk) lefiLayer_setSlotWireLength( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2012, "SLOTWIRELENGTH statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SLOTWIRELENGTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1565, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrLayerCbk) lefiLayer_setSlotWireLength( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 153:
#line 2247 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 if (lefrLayerCbk) lefiLayer_setSlotWidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2013, "SLOTWIDTH statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SLOTWIDTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1566, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrLayerCbk) lefiLayer_setSlotWidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 154:
#line 2277 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 if (lefrLayerCbk) lefiLayer_setSlotLength( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2014, "SLOTLENGTH statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SLOTLENGTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1567, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrLayerCbk) lefiLayer_setSlotLength( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 155:
#line 2307 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 if (lefrLayerCbk) lefiLayer_setMaxAdjacentSlotSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2015, "MAXADJACENTSLOTSPACING statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MAXADJACENTSLOTSPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1568, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrLayerCbk) lefiLayer_setMaxAdjacentSlotSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 156:
#line 2337 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 if (lefrLayerCbk) lefiLayer_setMaxCoaxialSlotSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2016, "MAXCOAXIALSLOTSPACING statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MAXCOAXIALSLOTSPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1569, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrLayerCbk) lefiLayer_setMaxCoaxialSlotSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 157:
#line 2367 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 if (lefrLayerCbk) lefiLayer_setMaxEdgeSlotSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2017, "MAXEDGESLOTSPACING statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MAXEDGESLOTSPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1570, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrLayerCbk) lefiLayer_setMaxEdgeSlotSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 158:
#line 2397 "lef.y"
    { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.7) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2018, "SPLITWIREWIDTH statement is obsolete in version 5.7 or later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.7 or later.");
 }
 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SPLITWIREWIDTH statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1571, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setSplitWireWidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 159:
#line 2425 "lef.y"
    { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MINIMUMDENSITY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1572, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMinimumDensity( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 160:
#line 2447 "lef.y"
    { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MAXIMUMDENSITY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1573, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMaximumDensity( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 161:
#line 2469 "lef.y"
    { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "DENSITYCHECKWINDOW statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1574, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setDensityCheckWindow( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 162:
#line 2492 "lef.y"
    { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "DENSITYCHECKSTEP statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1575, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setDensityCheckStep( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 163:
#line 2514 "lef.y"
    { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "FILLACTIVESPACING statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1576, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setFillActiveSpacing( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 164:
#line 2537 "lef.y"
    {
 /* 5.5 MAXWIDTH, is for routing layer only
 */
if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1577, "MAXWIDTH statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MAXWIDTH statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1578, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMaxwidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 165:
#line 2567 "lef.y"
    {
 /* 5.5 MINWIDTH, is for routing layer only
 */
if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1579, "MINWIDTH statement can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MINWIDTH statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1580, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMinwidth( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 166:
#line 2597 "lef.y"
    {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MINENCLOSEDAREA statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1581, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addMinenclosedarea( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 168:
#line 2617 "lef.y"
    { /* pcr 409334 */

 if (lefrLayerCbk)
 lefiLayer_addMinimumcut( &(lefrLayer),
 (int)(yyvsp[(2) - (4)].dval),(yyvsp[(4) - (4)].dval)) ;
 hasLayerMincut = 0;
 ;}
    break;

  case 169:
#line 2627 "lef.y"
    {
 if (!hasLayerMincut) { /* FROMABOVE nor FROMBELOW is set */

 if (lefrLayerCbk)
 lefiLayer_addMinimumcutConnect( &(lefrLayer),
 (char*)"") ;
 }
 ;}
    break;

  case 170:
#line 2637 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_addMinstep( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 171:
#line 2643 "lef.y"
    {
 ;}
    break;

  case 172:
#line 2647 "lef.y"
    {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "PROTRUSION RULE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1582, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setProtrusion( &(lefrLayer),
 (yyvsp[(2) - (7)].dval),
(yyvsp[(4) - (7)].dval),
(yyvsp[(6) - (7)].dval)) ;
 ;}
    break;

  case 173:
#line 2668 "lef.y"
    {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SPACINGTABLE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1583, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 /* 5.5 either SPACING or SPACINGTABLE in a layer, not both
 */
if (lefrHasSpacing && layerRout && versionNum < 5.7) {
 if (lefrLayerCbk) /* write warning only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefWarning(2010, "It is illegal to have both SPACING rules & SPACINGTABLE rules within a ROUTING layer");
 }
 }
 if (lefrLayerCbk) lefiLayer_addSpacingTable( &(lefrLayer) ) ;
 lefrHasSpacingTbl = 1;
 ;}
    break;

  case 175:
#line 2698 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ENCLOSURE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1584, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_addEnclosure( &(lefrLayer),
 (yyvsp[(2) - (4)].string),
(yyvsp[(3) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 177:
#line 2722 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "PREFERENCLOSURE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1585, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_addPreferEnclosure( &(lefrLayer),
 (yyvsp[(2) - (4)].string),
(yyvsp[(3) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 179:
#line 2744 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "RESISTANCE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1586, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else {
 if (lefrLayerCbk)
 lefiLayer_setResPerCut( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 180:
#line 2764 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1587, "DIAGMINEDGELENGTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions.");
 CHKERR();
 }
 }
 } else if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "DIAGMINEDGELENGTH statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1588, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else {
 if (lefrLayerCbk)
 lefiLayer_setDiagMinEdgeLength( &(lefrLayer),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 181:
#line 2792 "lef.y"
    {
 /* Use the polygon code to retrieve the points for MINSIZE
 */
lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
 lefiGeometries_Init( lefrGeometriesPtr ) ;
 lefrDoGeometries = 1;
 ;}
    break;

  case 182:
#line 2800 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiGeometries_addPolygon( lefrGeometriesPtr ) ;
 lefiLayer_setMinSize( &(lefrLayer),
 lefrGeometriesPtr) ;
 }
 lefrDoGeometries = 0;
 lefiGeometries_Destroy( lefrGeometriesPtr ) ;
 /* Don't need the object anymore
 */
lefFree(lefrGeometriesPtr);
 ;}
    break;

  case 184:
#line 2818 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setArraySpacingLongArray( &(lefrLayer) ) ;
 ;}
    break;

  case 186:
#line 2828 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setArraySpacingWidth( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 189:
#line 2842 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_addArraySpacingArray( &(lefrLayer),
 (int)(yyvsp[(2) - (4)].dval),(yyvsp[(4) - (4)].dval)) ;
 if (arrayCutsVal > (int)(yyvsp[(2) - (4)].dval)) {
 /* Mulitiple ARRAYCUTS value needs to me in ascending order */

 if (!arrayCutsWar) {
 if (layerWarnings++ < lefrLayerWarnings)
 lefWarning(2080, "The number of cut values in multiple ARRAYSPACING ARRAYCUTS are not in increasing order.\nTo be consistent with the documentation, update the cut values to increasing order.");
 arrayCutsWar = 1;
 }
 }
 arrayCutsVal = (int)(yyvsp[(2) - (4)].dval);
 }
 ;}
    break;

  case 190:
#line 2861 "lef.y"
    {
 if (hasInfluence) { /* 5.5 - INFLUENCE table must follow a PARALLEL
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1589, "An INFLUENCE table statement was defined before the PARALLELRUNLENGTH table statement.\nINFLUENCE table statement should be defined following the PARALLELRUNLENGTH.\nChange the LEF file and rerun the parser.");
 CHKERR();
 }
 }
 }
 if (hasParallel) { /* 5.5 - Only one PARALLEL table is allowed per layer
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1590, "There is multiple PARALLELRUNLENGTH table statements are defined within a layer.\nAccording to the LEF Reference Manual, only one PARALLELRUNLENGTH table statement is allowed per layer.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 hasParallel = 1;
 ;}
    break;

  case 191:
#line 2887 "lef.y"
    {
 spParallelLength = lefiLayer_getNumber( &(lefrLayer) ) ;
 if (lefrLayerCbk) lefiLayer_addSpParallelLength( &(lefrLayer) ) ;
 ;}
    break;

  case 192:
#line 2892 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_addSpParallelWidth( &(lefrLayer),
 (yyvsp[(7) - (7)].dval)) ;
 }
 ;}
    break;

  case 193:
#line 2899 "lef.y"
    {
 if (lefiLayer_getNumber( &(lefrLayer) )!=spParallelLength) { if (lefrLayerCbk) { if (layerWarnings++ < lefrLayerWarnings) { lefError(1591, "The number of length in PARALLELRUNLENGTH is not the same as the number of spacing in WIDTH.") ;
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addSpParallelWidthSpacing( &(lefrLayer) ) ;
 ;}
    break;

  case 195:
#line 2910 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(5) - (5)].dval)) ;
 ;}
    break;

  case 196:
#line 2915 "lef.y"
    {
 if (hasParallel) { /* 5.7 - Either PARALLEL OR TWOWIDTHS per layer
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1592, "A PARALLELRUNLENGTH statement has already defined in the layer.\nOnly either PARALLELRUNLENGTH or TWOWIDTHS is allowed per layer.");
 CHKERR();
 }
 }
 }
 if (hasTwoWidths) { /* 5.7 - only 1 TWOWIDTHS per layer
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1593, "A TWOWIDTHS table statement has already defined in the layer.\nOnly one TWOWIDTHS statement is allowed per layer.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addSpTwoWidths( &(lefrLayer),
 (yyvsp[(3) - (7)].dval),
(yyvsp[(4) - (7)].dval)) ;
 hasTwoWidths = 1;
 ;}
    break;

  case 197:
#line 2942 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "TWOWIDTHS is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1697, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 ;}
    break;

  case 198:
#line 2953 "lef.y"
    {
 if (hasInfluence) { /* 5.5 - INFLUENCE table must follow a PARALLEL
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1594, "A INFLUENCE table statement has already defined in the layer.\nOnly one INFLUENCE statement is allowed per layer.");
 CHKERR();
 }
 }
 }
 if (!hasParallel) { /* 5.5 - INFLUENCE must follow a PARALLEL
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1595, "An INFLUENCE table statement has already defined beofre the layer.\nINFLUENCE statement has to be defined after the PARALLELRUNLENGTH table statement in the layer.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_setInfluence( &(lefrLayer) ) ;
 lefiLayer_addSpInfluence( &(lefrLayer),
 (yyvsp[(3) - (7)].dval),
(yyvsp[(5) - (7)].dval),
(yyvsp[(7) - (7)].dval)) ;
 }
 ;}
    break;

  case 202:
#line 2992 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_addSpacingTableOrthoWithin( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 203:
#line 3000 "lef.y"
    {(yyval.string) = (char*)"NULL";;}
    break;

  case 204:
#line 3002 "lef.y"
    {(yyval.string) = (char*)"ABOVE";;}
    break;

  case 205:
#line 3003 "lef.y"
    {(yyval.string) = (char*)"BELOW";;}
    break;

  case 207:
#line 3008 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_addEnclosureWidth( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 ;}
    break;

  case 209:
#line 3017 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "LENGTH is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1691, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk) {
 lefiLayer_addEnclosureLength( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 }
 ;}
    break;

  case 211:
#line 3037 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "EXCEPTEXTRACUT is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1690, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk) {
 lefiLayer_addEnclosureExceptEC( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 }
 ;}
    break;

  case 213:
#line 3056 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_addPreferEnclosureWidth( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 ;}
    break;

  case 215:
#line 3066 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "MINIMUMCUT WITHIN is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1700, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk) {
 lefiLayer_addMinimumcutWithin( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 }
 ;}
    break;

  case 217:
#line 3085 "lef.y"
    {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "FROMABOVE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1596, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 hasLayerMincut = 1;
 if (lefrLayerCbk)
 lefiLayer_addMinimumcutConnect( &(lefrLayer),
 (char*)"FROMABOVE") ;
 
 ;}
    break;

  case 218:
#line 3106 "lef.y"
    {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "FROMBELOW statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1597, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 hasLayerMincut = 1;
 if (lefrLayerCbk)
 lefiLayer_addMinimumcutConnect( &(lefrLayer),
 (char*)"FROMBELOW") ;
 ;}
    break;

  case 220:
#line 3129 "lef.y"
    {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "LENGTH WITHIN statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1598, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_addMinimumcutLengDis( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 223:
#line 3160 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_addMinstepType( &(lefrLayer),
 (yyvsp[(1) - (1)].string)) ;
 ;}
    break;

  case 224:
#line 3165 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_addMinstepLengthsum( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 225:
#line 3171 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "MAXEDGES is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1685, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else
 if (lefrLayerCbk) lefiLayer_addMinstepMaxedges( &(lefrLayer),
 (int)(yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 226:
#line 3185 "lef.y"
    {(yyval.string) = (char*)"INSIDECORNER";;}
    break;

  case 227:
#line 3186 "lef.y"
    {(yyval.string) = (char*)"OUTSIDECORNER";;}
    break;

  case 228:
#line 3187 "lef.y"
    {(yyval.string) = (char*)"STEP";;}
    break;

  case 229:
#line 3191 "lef.y"
    { if (lefrLayerCbk)
 lefiLayer_setAntennaValue( &(lefrLayer),
 antennaType,
(yyvsp[(1) - (1)].dval)) ;
 ;}
    break;

  case 230:
#line 3197 "lef.y"
    { if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */

 lefrAntennaPWLPtr = (lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
 lefiAntennaPWL_Init( lefrAntennaPWLPtr ) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 (yyvsp[(3) - (4)].pt).x,
(yyvsp[(3) - (4)].pt).y) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 (yyvsp[(4) - (4)].pt).x,
(yyvsp[(4) - (4)].pt).y) ;
 }
 ;}
    break;

  case 231:
#line 3210 "lef.y"
    { if (lefrLayerCbk)
 lefiLayer_setAntennaPWL( &(lefrLayer),
 antennaType,
lefrAntennaPWLPtr) ;
 ;}
    break;

  case 234:
#line 3228 "lef.y"
    { if (lefrLayerCbk)
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 (yyvsp[(1) - (1)].pt).x,
(yyvsp[(1) - (1)].pt).y) ;
 ;}
    break;

  case 236:
#line 3237 "lef.y"
    {
 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 }
 else if ((antennaType == lefiAntennaAF) && (versionNum <= 5.3)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAAREAFACTOR with DIFFUSEONLY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1599, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAAREAFACTOR with DIFFUSEONLY statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1599, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setAntennaDUO( &(lefrLayer),
 antennaType) ;
 ;}
    break;

  case 237:
#line 3274 "lef.y"
    {(yyval.string) = (char*)"PEAK";;}
    break;

  case 238:
#line 3275 "lef.y"
    {(yyval.string) = (char*)"AVERAGE";;}
    break;

  case 239:
#line 3276 "lef.y"
    {(yyval.string) = (char*)"RMS";;}
    break;

  case 240:
#line 3280 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 241:
#line 3284 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addAcFrequency( &(lefrLayer) ) ;
 ;}
    break;

  case 242:
#line 3288 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(9) - (9)].dval)) ;
 ;}
    break;

  case 243:
#line 3292 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addAcTableEntry( &(lefrLayer) ) ;
 ;}
    break;

  case 245:
#line 3298 "lef.y"
    {
 if (!layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1600, "CUTAREA statement can only be defined in LAYER with type CUT.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 246:
#line 3312 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addAcCutarea( &(lefrLayer) ) ;
 ;}
    break;

  case 247:
#line 3315 "lef.y"
    {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1601, "WIDTH can only be defined in LAYER with type ROUTING.");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 248:
#line 3329 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addAcWidth( &(lefrLayer) ) ;
 ;}
    break;

  case 249:
#line 3334 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 250:
#line 3338 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addDcTableEntry( &(lefrLayer) ) ;
 ;}
    break;

  case 252:
#line 3343 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 255:
#line 3354 "lef.y"
    {
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiLayer_addProp( &(lefrLayer),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 256:
#line 3366 "lef.y"
    {
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiLayer_addProp( &(lefrLayer),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 257:
#line 3378 "lef.y"
    {
 char temp[32];
 sprintf(temp, "%.11g", (yyvsp[(2) - (2)].dval));
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiLayer_addNumProp( &(lefrLayer),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].dval),
temp,
propTp) ;
 }
 ;}
    break;

  case 258:
#line 3395 "lef.y"
    { ;}
    break;

  case 259:
#line 3397 "lef.y"
    { ;}
    break;

  case 260:
#line 3400 "lef.y"
    { if (lefrLayerCbk) lefiLayer_setCurrentPoint( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 263:
#line 3411 "lef.y"
    { if (lefrLayerCbk) lefiLayer_setCapacitancePoint( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 265:
#line 3419 "lef.y"
    { ;}
    break;

  case 266:
#line 3422 "lef.y"
    { if (lefrLayerCbk) lefiLayer_setResistancePoint( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 267:
#line 3428 "lef.y"
    {(yyval.string) = (char*)"ROUTING"; layerRout = 1;;}
    break;

  case 268:
#line 3429 "lef.y"
    {(yyval.string) = (char*)"CUT"; layerCut = 1;;}
    break;

  case 269:
#line 3430 "lef.y"
    {(yyval.string) = (char*)"OVERLAP"; layerMastOver = 1;;}
    break;

  case 270:
#line 3431 "lef.y"
    {(yyval.string) = (char*)"MASTERSLICE"; layerMastOver = 1;;}
    break;

  case 271:
#line 3432 "lef.y"
    {(yyval.string) = (char*)"VIRTUAL";;}
    break;

  case 272:
#line 3433 "lef.y"
    {(yyval.string) = (char*)"IMPLANT";;}
    break;

  case 273:
#line 3436 "lef.y"
    {(yyval.string) = (char*)"HORIZONTAL";;}
    break;

  case 274:
#line 3437 "lef.y"
    {(yyval.string) = (char*)"VERTICAL";;}
    break;

  case 275:
#line 3438 "lef.y"
    {(yyval.string) = (char*)"DIAG45";;}
    break;

  case 276:
#line 3439 "lef.y"
    {(yyval.string) = (char*)"DIAG135";;}
    break;

  case 278:
#line 3443 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_addMinenclosedareaWidth( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 279:
#line 3451 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 1) ;
 ;}
    break;

  case 280:
#line 3457 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 2) ;
 ;}
    break;

  case 281:
#line 3463 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 3) ;
 ;}
    break;

  case 282:
#line 3469 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 4) ;
 ;}
    break;

  case 283:
#line 3477 "lef.y"
    { ;}
    break;

  case 284:
#line 3484 "lef.y"
    { ;}
    break;

  case 285:
#line 3487 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_addSpParallelWidth( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 ;}
    break;

  case 286:
#line 3494 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addSpParallelWidthSpacing( &(lefrLayer) ) ;
 ;}
    break;

  case 287:
#line 3500 "lef.y"
    { ;}
    break;

  case 288:
#line 3502 "lef.y"
    { ;}
    break;

  case 289:
#line 3520 "lef.y"
    {
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 (yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 290:
#line 3525 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_addSpTwoWidths( &(lefrLayer),
 (yyvsp[(2) - (6)].dval),
(yyvsp[(3) - (6)].dval)) ;
 ;}
    break;

  case 291:
#line 3534 "lef.y"
    { (yyval.dval) = -1; ;}
    break;

  case 292:
#line 3537 "lef.y"
    { (yyval.dval) = (yyvsp[(2) - (2)].dval); ;}
    break;

  case 293:
#line 3541 "lef.y"
    { ;}
    break;

  case 294:
#line 3543 "lef.y"
    { ;}
    break;

  case 295:
#line 3546 "lef.y"
    { if (lefrLayerCbk) lefiLayer_addSpInfluence( &(lefrLayer),
 (yyvsp[(2) - (6)].dval),
(yyvsp[(4) - (6)].dval),
(yyvsp[(6) - (6)].dval)) ;
 ;}
    break;

  case 296:
#line 3553 "lef.y"
    {
 if (!lefrHasLayer) { /* 5.5 */

 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 lefError(1602, "MAXVIASTACK statement has to be defined after the LAYER statement.");
 CHKERR();
 }
 }
 } else if (lefrHasMaxVS) {
 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 lefError(1603, "A MAXVIASTACK has already defined.\nOnly one MAXVIASTACK is allowed per lef file.");
 CHKERR();
 }
 }
 } else {
 if (lefrMaxStackViaCbk) {
 lefiMaxStackVia_setMaxStackVia( &(lefrMaxStackVia),
 (int)(yyvsp[(2) - (3)].dval)) ;
 CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
 }
 }
 if (versionNum < 5.5) {
 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MAXVIASTACK statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1604, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 lefrHasMaxVS = 1;
 ;}
    break;

  case 297:
#line 3593 "lef.y"
    {lefDumbMode = 2; lefNoNum= 2;;}
    break;

  case 298:
#line 3595 "lef.y"
    {
 if (!lefrHasLayer) { /* 5.5 */

 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 lefError(1602, "MAXVIASTACK statement has to be defined after the LAYER statement.");
 CHKERR();
 }
 }
 } else if (lefrHasMaxVS) {
 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 lefError(1603, "A MAXVIASTACK has already defined.\nOnly one MAXVIASTACK is allowed per lef file.");
 CHKERR();
 }
 }
 } else {
 if (lefrMaxStackViaCbk) {
 lefiMaxStackVia_setMaxStackVia( &(lefrMaxStackVia),
 (int)(yyvsp[(2) - (7)].dval)) ;
 lefiMaxStackVia_setMaxStackViaRange( &(lefrMaxStackVia),
 (yyvsp[(5) - (7)].string),
(yyvsp[(6) - (7)].string)) ;
 CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
 }
 }
 lefrHasMaxVS = 1;
 ;}
    break;

  case 299:
#line 3626 "lef.y"
    { hasViaRule_layer = 0; ;}
    break;

  case 300:
#line 3627 "lef.y"
    {
 if (lefrViaCbk)
 CALLBACK(lefrViaCbk, lefrViaCbkType, &lefrVia);
 ;}
    break;

  case 301:
#line 3634 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 302:
#line 3637 "lef.y"
    {
 /* 0 is nodefault */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 (yyvsp[(2) - (2)].string),
0) ;
 viaLayer = 0;
 numVia++;
 /* $2);
  */
viaName = strdup((yyvsp[(2) - (2)].string));
 ;}
    break;

  case 303:
#line 3650 "lef.y"
    {
 /* 1 is default */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 (yyvsp[(2) - (3)].string),
1) ;
 viaLayer = 0;
 /* $2);
  */
viaName = strdup((yyvsp[(2) - (3)].string));
 ;}
    break;

  case 304:
#line 3662 "lef.y"
    {
 /* 2 is generated */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 (yyvsp[(2) - (3)].string),
2) ;
 viaLayer = 0;
 /* $2);
  */
viaName = strdup((yyvsp[(2) - (3)].string));
 ;}
    break;

  case 305:
#line 3674 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 306:
#line 3676 "lef.y"
    {lefDumbMode = 3; lefNoNum = 1; ;}
    break;

  case 307:
#line 3679 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrViaCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "VIARULE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1605, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrViaCbk) lefiVia_setViaRule( &(lefrVia),
 (yyvsp[(3) - (24)].string),
(yyvsp[(6) - (24)].dval),
(yyvsp[(7) - (24)].dval),
(yyvsp[(11) - (24)].string),
(yyvsp[(12) - (24)].string),
(yyvsp[(13) - (24)].string),
(yyvsp[(16) - (24)].dval),
(yyvsp[(17) - (24)].dval),
(yyvsp[(20) - (24)].dval),
(yyvsp[(21) - (24)].dval),
(yyvsp[(22) - (24)].dval),
(yyvsp[(23) - (24)].dval)) ;
 viaLayer++;
 hasViaRule_layer = 1;
 ;}
    break;

  case 311:
#line 3718 "lef.y"
    {
 if (lefrViaCbk) lefiVia_setRowCol( &(lefrVia),
 (int)(yyvsp[(2) - (4)].dval),(int)(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 312:
#line 3723 "lef.y"
    {
 if (lefrViaCbk) lefiVia_setOrigin( &(lefrVia),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 313:
#line 3729 "lef.y"
    {
 if (lefrViaCbk) lefiVia_setOffset( &(lefrVia),
 (yyvsp[(2) - (6)].dval),
(yyvsp[(3) - (6)].dval),
(yyvsp[(4) - (6)].dval),
(yyvsp[(5) - (6)].dval)) ;
 ;}
    break;

  case 314:
#line 3736 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 315:
#line 3737 "lef.y"
    {
 if (lefrViaCbk) lefiVia_setPattern( &(lefrVia),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 321:
#line 3756 "lef.y"
    { ;}
    break;

  case 322:
#line 3758 "lef.y"
    { ;}
    break;

  case 323:
#line 3760 "lef.y"
    { if (lefrViaCbk) lefiVia_setResistance( &(lefrVia),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 324:
#line 3763 "lef.y"
    { lefDumbMode = 1000000; lefRealNum = 1; lefInProp = 1; ;}
    break;

  case 325:
#line 3764 "lef.y"
    { lefDumbMode = 0;
 lefRealNum = 0;
 lefInProp = 0;
 ;}
    break;

  case 326:
#line 3769 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setTopOfStack( &(lefrVia) ) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 lefWarning(2019, "TOPOFSTACKONLY statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later");
 ;}
    break;

  case 329:
#line 3786 "lef.y"
    {
 char temp[32];
 sprintf(temp, "%.11g", (yyvsp[(2) - (2)].dval));
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiVia_addNumProp( &(lefrVia),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].dval),
temp,
propTp) ;
 }
 ;}
    break;

  case 330:
#line 3801 "lef.y"
    {
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiVia_addProp( &(lefrVia),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 331:
#line 3813 "lef.y"
    {
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiVia_addProp( &(lefrVia),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 332:
#line 3827 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 (yyvsp[(1) - (2)].string),
0,
0.0,
0.0,
-1) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 lefWarning(2020, "FOREIGN statement in VIA is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 333:
#line 3842 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 (yyvsp[(1) - (3)].string),
1,
(yyvsp[(2) - (3)].pt).x,
(yyvsp[(2) - (3)].pt).y,
-1) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 lefWarning(2020, "FOREIGN statement in VIA is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 334:
#line 3857 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 (yyvsp[(1) - (4)].string),
1,
(yyvsp[(2) - (4)].pt).x,
(yyvsp[(2) - (4)].pt).y,
(yyvsp[(3) - (4)].integer)) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 lefWarning(2020, "FOREIGN statement in VIA is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 335:
#line 3872 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 (yyvsp[(1) - (3)].string),
0,
0.0,
0.0,
(yyvsp[(2) - (3)].integer)) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 lefWarning(2020, "FOREIGN statement in VIA is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 336:
#line 3887 "lef.y"
    {lefDumbMode = 1; lefNoNum= 1;;}
    break;

  case 337:
#line 3888 "lef.y"
    { (yyval.string) = (yyvsp[(3) - (3)].string); ;}
    break;

  case 338:
#line 3891 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 339:
#line 3892 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 340:
#line 3893 "lef.y"
    {(yyval.integer) = 2;;}
    break;

  case 341:
#line 3894 "lef.y"
    {(yyval.integer) = 3;;}
    break;

  case 342:
#line 3895 "lef.y"
    {(yyval.integer) = 4;;}
    break;

  case 343:
#line 3896 "lef.y"
    {(yyval.integer) = 5;;}
    break;

  case 344:
#line 3897 "lef.y"
    {(yyval.integer) = 6;;}
    break;

  case 345:
#line 3898 "lef.y"
    {(yyval.integer) = 7;;}
    break;

  case 346:
#line 3899 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 347:
#line 3900 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 348:
#line 3901 "lef.y"
    {(yyval.integer) = 2;;}
    break;

  case 349:
#line 3902 "lef.y"
    {(yyval.integer) = 3;;}
    break;

  case 350:
#line 3903 "lef.y"
    {(yyval.integer) = 4;;}
    break;

  case 351:
#line 3904 "lef.y"
    {(yyval.integer) = 5;;}
    break;

  case 352:
#line 3905 "lef.y"
    {(yyval.integer) = 6;;}
    break;

  case 353:
#line 3906 "lef.y"
    {(yyval.integer) = 7;;}
    break;

  case 354:
#line 3909 "lef.y"
    { ;}
    break;

  case 355:
#line 3911 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 356:
#line 3912 "lef.y"
    {
 if (lefrViaCbk) lefiVia_addLayer( &(lefrVia),
 (yyvsp[(3) - (4)].string)) ;
 viaLayer++;
 hasViaRule_layer = 1;
 ;}
    break;

  case 359:
#line 3927 "lef.y"
    { if (lefrViaCbk)
 lefiVia_addRectToLayer( &(lefrVia),
 (yyvsp[(2) - (4)].pt).x,
(yyvsp[(2) - (4)].pt).y,
(yyvsp[(3) - (4)].pt).x,
(yyvsp[(3) - (4)].pt).y) ;
 ;}
    break;

  case 360:
#line 3936 "lef.y"
    {
 lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
 lefiGeometries_Init( lefrGeometriesPtr ) ;
 lefrDoGeometries = 1;
 ;}
    break;

  case 361:
#line 3942 "lef.y"
    {
 if (lefrViaCbk) {
 lefiGeometries_addPolygon( lefrGeometriesPtr ) ;
 lefiVia_addPolyToLayer( &(lefrVia),
 lefrGeometriesPtr) ;
 /* 5.6
  */
}
 lefiGeometries_clearPolyItems( lefrGeometriesPtr ) ;
 /* free items fields
 */
lefFree((char*)(lefrGeometriesPtr)); /* Don't need anymore, poly data has
 */
lefrDoGeometries = 0; /* copied
  */
;}
    break;

  case 362:
#line 3959 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 363:
#line 3960 "lef.y"
    {
 /* 10/17/2001 - Wanda da Rosa, PCR 404149
 */
/*              Error if no layer in via
 */
if (!viaLayer) {
 if (lefrViaCbk) { /* write error only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "A LAYER statement is missing in the VIA %s.\nAt least one LAYERis required per VIA statement.", (yyvsp[(3) - (3)].string));
 lefError(1606, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (strcmp(viaName, (yyvsp[(3) - (3)].string)) != 0) {
 if (lefrViaCbk) { /* write error only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END VIA name %s is different from the VIA name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (3)].string), viaName);
 lefError(1607, outMsg);
 lefFree(outMsg);
 lefFree(viaName);
 CHKERR();
 } else
 lefFree(viaName);
 } else
 lefFree(viaName);
 } else
 lefFree(viaName);
 ;}
    break;

  case 364:
#line 3997 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 365:
#line 3998 "lef.y"
    {
 if (lefrViaRuleCbk) lefiViaRule_setName( &(lefrViaRule),
 (yyvsp[(3) - (3)].string)) ;
 viaRuleLayer = 0;
 /* $3);
  */
viaRuleName = strdup((yyvsp[(3) - (3)].string));
 isGenerate = 0;
 ;}
    break;

  case 366:
#line 4010 "lef.y"
    {
 /* 10/17/2001 - Wanda da Rosa, PCR 404163
 */
/*              Error if layer number is not equal 2.
 */
/* 11/14/2001 - Wanda da Rosa,
 */
/*              Commented out for pcr 411781
 */
/* (viaRuleLayer != 2) {
 */
/* requires two layers");
 */
if (viaRuleLayer == 0 || viaRuleLayer > 2) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1608, "A VIARULE statement requires two layers.");
 CHKERR();
 }
 }
 }
 if (lefrViaRuleCbk)
 CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
 /* 2/19/2004 - reset the ENCLOSURE overhang values which may be
 */
/* set by the old syntax OVERHANG -- Not necessary, but just incase
 */
if (lefrViaRuleCbk) lefiViaRule_clearLayerOverhang( &(lefrViaRule) ) ;
 ;}
    break;

  case 367:
#line 4043 "lef.y"
    {
 isGenerate = 1;
 ;}
    break;

  case 368:
#line 4047 "lef.y"
    {
 /* 10/17/2001 - Wanda da Rosa, PCR 404181
 */
/*              Error if layer number is not equal 3.
 */
/* 11/14/2001 - Wanda da Rosa,
 */
/*              Commented out for pcr 411781
 */
/* (viaRuleLayer != 3) {
 */
/* requires three layers");
 */
if (viaRuleLayer == 0) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1609, "A VIARULE GENERATE requires three layers.");
 CHKERR();
 }
 }
 } else if ((viaRuleLayer < 3) && (versionNum >= 5.6)) {
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 lefWarning(2021, "turn-via is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 } else {
 if (lefrViaRuleCbk) {
 lefiViaRule_setGenerate( &(lefrViaRule) ) ;
 CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
 }
 }
 /* 2/19/2004 - reset the ENCLOSURE overhang values which may be
 */
/* set by the old syntax OVERHANG
 */
if (lefrViaRuleCbk) lefiViaRule_clearLayerOverhang( &(lefrViaRule) ) ;
 ;}
    break;

  case 370:
#line 4090 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "DEFAULT statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1605, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrViaRuleCbk) lefiViaRule_setDefault( &(lefrViaRule) ) ;
 ;}
    break;

  case 377:
#line 4123 "lef.y"
    { lefDumbMode = 10000000; lefRealNum = 1; lefInProp = 1; ;}
    break;

  case 378:
#line 4124 "lef.y"
    { lefDumbMode = 0;
 lefRealNum = 0;
 lefInProp = 0;
 ;}
    break;

  case 381:
#line 4136 "lef.y"
    {
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiViaRule_addProp( &(lefrViaRule),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 382:
#line 4148 "lef.y"
    {
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiViaRule_addProp( &(lefrViaRule),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 383:
#line 4160 "lef.y"
    {
 char temp[32];
 sprintf(temp, "%.11g", (yyvsp[(2) - (2)].dval));
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiViaRule_addNumProp( &(lefrViaRule),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].dval),
temp,
propTp) ;
 }
 ;}
    break;

  case 384:
#line 4176 "lef.y"
    {
 /* 10/18/2001 - Wanda da Rosa PCR 404181
 */
/*              Make sure the 1st 2 layers in viarule has direction
 */
/* 04/28/2004 - PCR 704072 - DIRECTION in viarule generate is
 */
/*              obsolete in 5.6
 */
if (versionNum >= 5.6) {
 if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
 !isGenerate) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1606, "VIARULE statement in a layer, requires a DIRECTION construct statement.");
 CHKERR();
 }
 }
 }
 } else {
 if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
 isGenerate) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1606, "VIARULE statement in a layer, requires a DIRECTION construct statement.");
 CHKERR();
 }
 }
 }
 }
 viaRuleLayer++;
 ;}
    break;

  case 387:
#line 4219 "lef.y"
    { if (lefrViaRuleCbk) lefiViaRule_addViaName( &(lefrViaRule),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 388:
#line 4223 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 389:
#line 4224 "lef.y"
    { if (lefrViaRuleCbk) lefiViaRule_setLayer( &(lefrViaRule),
 (yyvsp[(3) - (4)].string)) ;
 viaRuleHasDir = 0;
 viaRuleHasEnc = 0;
 ;}
    break;

  case 392:
#line 4238 "lef.y"
    {
 if (viaRuleHasEnc) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1607, "An ENCLOSRE statement has already defined in the layer.\nOnly either DIRECTION or ENCLOSURE can be specified in a layer.");
 CHKERR();
 }
 }
 } else {
 if ((versionNum < 5.6) || (!isGenerate)) {
 if (lefrViaRuleCbk) lefiViaRule_setHorizontal( &(lefrViaRule) ) ;
 } else
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 lefWarning(2022, "DIRECTION statement in VIARULE is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 }
 viaRuleHasDir = 1;
 ;}
    break;

  case 393:
#line 4259 "lef.y"
    {
 if (viaRuleHasEnc) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1607, "An ENCLOSRE statement has already defined in the layer.\nOnly either DIRECTION or ENCLOSURE can be specified in a layer.");
 CHKERR();
 }
 }
 } else {
 if ((versionNum < 5.6) || (!isGenerate)) {
 if (lefrViaRuleCbk) lefiViaRule_setVertical( &(lefrViaRule) ) ;
 } else
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 lefWarning(2022, "DIRECTION statement in VIARULE is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 }
 viaRuleHasDir = 1;
 ;}
    break;

  case 394:
#line 4281 "lef.y"
    {
 if (versionNum < 5.5) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ENCLOSURE statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1608, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 /* 2/19/2004 - Enforced the rule that ENCLOSURE can only be defined
 */
/* in VIARULE GENERATE
 */
if (!isGenerate) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1614, "An ENCLOSURE statement is defined in a VIARULE statement only.\nOVERHANG statement can only be defined in VIARULE GENERATE.");
 CHKERR();
 }
 }
 }
 if (viaRuleHasDir) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1609, "A DIRECTION statement has already defined in the layer.\nOnly either DIRECTION or ENCLOSURE can be specified in a layer.");
 CHKERR();
 }
 }
 } else {
 if (lefrViaRuleCbk) lefiViaRule_setEnclosure( &(lefrViaRule),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 }
 viaRuleHasEnc = 1;
 ;}
    break;

  case 395:
#line 4324 "lef.y"
    { if (lefrViaRuleCbk) lefiViaRule_setWidth( &(lefrViaRule),
 (yyvsp[(2) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 ;}
    break;

  case 396:
#line 4329 "lef.y"
    { if (lefrViaRuleCbk)
 lefiViaRule_setRect( &(	lefrViaRule),
 (yyvsp[(2) - (4)].pt).x,
(yyvsp[(2) - (4)].pt).y,
(yyvsp[(3) - (4)].pt).x,
(yyvsp[(3) - (4)].pt).y) ;
 ;}
    break;

  case 397:
#line 4337 "lef.y"
    { if (lefrViaRuleCbk) lefiViaRule_setSpacing( &(lefrViaRule),
 (yyvsp[(2) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 ;}
    break;

  case 398:
#line 4342 "lef.y"
    { if (lefrViaRuleCbk) lefiViaRule_setResistance( &(lefrViaRule),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 399:
#line 4346 "lef.y"
    {
 if (!viaRuleHasDir) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1610, "An OVERHANG statement is defined, but the required DIRECTION statement is not yet defined.\nUpdate the LEF file to define the DIRECTION statement before the OVERHANG.");
 CHKERR();
 }
 }
 }
 /* 2/19/2004 - Enforced the rule that OVERHANG can only be defined
 */
/* in VIARULE GENERATE after 5.3
 */
if ((versionNum > 5.3) && (!isGenerate)) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1611, "An OVERHANG statement is defined in a VIARULE statement only.\nOVERHANG statement can only be defined in VIARULE GENERATE.");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.6) {
 if (lefrViaRuleCbk) lefiViaRule_setOverhang( &(lefrViaRule),
 (yyvsp[(2) - (3)].dval)) ;
 } else {
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 lefWarning(2023, "OVERHANG statement will be translated into similar ENCLOSURE rule");
 /* In 5.6 & later, set it to either ENCLOSURE overhang1 or overhang2
 */
if (lefrViaRuleCbk) lefiViaRule_setOverhangToEnclosure( &(lefrViaRule),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 400:
#line 4384 "lef.y"
    {
 /* 2/19/2004 - Enforced the rule that METALOVERHANG can only be defined
 */
/* in VIARULE GENERATE
 */
if ((versionNum > 5.3) && (!isGenerate)) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1612, "An METALOVERHANG statement is defined in a VIARULE statement only.\nOVERHANG statement can only be defined in VIARULE GENERATE.");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.6) {
 if (!viaRuleHasDir) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 lefError(1613, "An METALOVERHANG statement is defined, but the required DIRECTION statement is not yet defined.\nUpdate the LEF file to define the DIRECTION statement before the OVERHANG.");
 CHKERR();
 }
 }
 }
 if (lefrViaRuleCbk) lefiViaRule_setMetalOverhang( &(lefrViaRule),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 lefWarning(2024, "METALOVERHANG statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 401:
#line 4417 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 402:
#line 4418 "lef.y"
    {
 if (strcmp(viaRuleName, (yyvsp[(3) - (3)].string)) != 0) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END VIARULE name %s is different from the VIARULE name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (3)].string), viaRuleName);
 lefError(1615, outMsg);
 lefFree(outMsg);
 lefFree(viaRuleName);
 CHKERR();
 } else
 lefFree(viaRuleName);
 } else
 lefFree(viaRuleName);
 } else
 lefFree(viaRuleName);
 ;}
    break;

  case 403:
#line 4439 "lef.y"
    { ;}
    break;

  case 404:
#line 4442 "lef.y"
    {
 hasSamenet = 0;
 if ((versionNum < 5.6) || (!ndRule)) {
 /* if 5.6 and in nondefaultrule, it should not get in here, */

 /* it should go to the else statement to write out a warning */

 /* if 5.6, not in nondefaultrule, it will get in here */

 /* if 5.5 and earlier in nondefaultrule is ok to get in here */

 if (versionNum >= 5.7) { /* will get to this if statement if */

 /* versionNum is 5.6 and higher but ndRule = 0 */

 if (spacingWarnings == 0) { /* only print once */

 lefWarning(2077, "A SPACING SAMENET section is defined but it is not legal in a LEF 5.7 version file.\nIt will be ignored which will probably cause real DRC violations to be ignored, and may\ncause false DRC violations to occur.\n\nTo avoid this warning, and correctly handle these DRC rules, you should modify your\nLEF to use the appropriate SAMENET keywords as described in the LEF/DEF 5.7\nmanual under the SPACING statements in the LAYER (Routing) and LAYER (Cut)\nsections listed in the LEF Table of Contents.");
 spacingWarnings++;
 }
 } else if (lefrSpacingBeginCbk)
 	 CALLBACK(lefrSpacingBeginCbk, lefrSpacingBeginCbkType, 0);
 } else
 if (lefrSpacingBeginCbk) /* write warning only if cbk is set */

 if (spacingWarnings++ < lefrSpacingWarnings)
 lefWarning(2025, "SAMENET statement in NONDEFAULTRULE is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 405:
#line 4472 "lef.y"
    {
 if ((versionNum < 5.6) || (!ndRule)) {
 if ((versionNum <= 5.4) && (!hasSamenet)) {
 lefError(1616, "SAMENET statement is required inside SPACING for any lef file with version 5.4 and earlier, but is not defined in the parsed lef file.");
 CHKERR();
 } else if (versionNum < 5.7) { /* obsolete in 5.7 and later */

 if (lefrSpacingEndCbk)
 CALLBACK(lefrSpacingEndCbk, lefrSpacingEndCbkType, 0);
 }
 }
 ;}
    break;

  case 408:
#line 4492 "lef.y"
    {
 if ((versionNum < 5.6) || (!ndRule)) {
 if (versionNum < 5.7) {
 if (lefrSpacingCbk) {
 lefiSpacing_set( &(lefrSpacing),
 (yyvsp[(2) - (5)].string),
(yyvsp[(3) - (5)].string),
(yyvsp[(4) - (5)].dval),
0) ;
 CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
 }
 }
 }
 ;}
    break;

  case 409:
#line 4507 "lef.y"
    {
 if ((versionNum < 5.6) || (!ndRule)) {
 if (versionNum < 5.7) {
 if (lefrSpacingCbk) {
 	 lefiSpacing_set( &(lefrSpacing),
 (yyvsp[(2) - (6)].string),
(yyvsp[(3) - (6)].string),
(yyvsp[(4) - (6)].dval),
1) ;
 	 CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
 }
 }
 }
 ;}
    break;

  case 410:
#line 4525 "lef.y"
    { lefDumbMode = 2; lefNoNum = 2; hasSamenet = 1; ;}
    break;

  case 411:
#line 4528 "lef.y"
    { ;}
    break;

  case 412:
#line 4531 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrIRDropBeginCbk)
 	 CALLBACK(lefrIRDropBeginCbk, lefrIRDropBeginCbkType, 0);
 } else
 if (lefrIRDropBeginCbk) /* write warning only if cbk is set */

 if (iRDropWarnings++ < lefrIRDropWarnings)
 lefWarning(2026, "IRDROP statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 413:
#line 4543 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrIRDropEndCbk)
 	 CALLBACK(lefrIRDropEndCbk, lefrIRDropEndCbkType, 0);
 }
 ;}
    break;

  case 416:
#line 4558 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrIRDropCbk)
 CALLBACK(lefrIRDropCbk, lefrIRDropCbkType, &lefrIRDrop);
 }
 ;}
    break;

  case 419:
#line 4572 "lef.y"
    { if (lefrIRDropCbk) lefiIRDrop_setValues( &(lefrIRDrop),
 (yyvsp[(1) - (2)].dval),
(yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 420:
#line 4578 "lef.y"
    { if (lefrIRDropCbk) lefiIRDrop_setTableName( &(lefrIRDrop),
 (yyvsp[(2) - (2)].string)) ;
 ;}
    break;

  case 421:
#line 4583 "lef.y"
    {
 hasMinfeature = 1;
 if (versionNum < 5.4) {
 if (lefrMinFeatureCbk) {
 lefiMinFeature_set( &(lefrMinFeature),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 CALLBACK(lefrMinFeatureCbk, lefrMinFeatureCbkType, &lefrMinFeature);
 }
 } else
 if (lefrMinFeatureCbk) /* write warning only if cbk is set */

 if (minFeatureWarnings++ < lefrMinFeatureWarnings)
 lefWarning(2027, "MINFEATURE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 422:
#line 4600 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrDielectricCbk)
 CALLBACK(lefrDielectricCbk, lefrDielectricCbkType, (yyvsp[(2) - (3)].dval));
 } else
 if (lefrDielectricCbk) /* write warning only if cbk is set */

 if (dielectricWarnings++ < lefrDielectricWarnings)
 lefWarning(2028, "DIELECTRIC statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 423:
#line 4611 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 424:
#line 4612 "lef.y"
    {
 (void)lefSetNonDefault((yyvsp[(3) - (3)].string));
 if (lefrNonDefaultCbk) lefiNonDefault_setName( &(lefrNonDefault),
 (yyvsp[(3) - (3)].string)) ;
 ndLayer = 0;
 ndRule = 1;
 numVia = 0;
 /* $3);
  */
nonDefaultRuleName = strdup((yyvsp[(3) - (3)].string));
 ;}
    break;

  case 425:
#line 4624 "lef.y"
    {lefNdRule = 1;;}
    break;

  case 426:
#line 4625 "lef.y"
    {
 /* 10/18/2001 - Wanda da Rosa, PCR 404189
 */
/*              At least 1 layer is required
 */
if ((!ndLayer) && (!lefrRelaxMode)) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 lefError(1617, "NONDEFAULTRULE statement requires at least one LAYER statement.");
 CHKERR();
 }
 }
 }
 if ((!numVia) && (!lefrRelaxMode) && (versionNum < 5.6)) {
 /* VIA is no longer a required statement in 5.6
 */
if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 lefError(1618, "NONDEFAULTRULE statement requires at least one VIA statement.");
 CHKERR();
 }
 }
 }
 if (lefrNonDefaultCbk) {
 lefiNonDefault_end( &(lefrNonDefault) ) ;
 CALLBACK(lefrNonDefaultCbk, lefrNonDefaultCbkType, &lefrNonDefault);
 }
 ndRule = 0;
 lefDumbMode = 0;
 (void)lefUnsetNonDefault();
 ;}
    break;

  case 427:
#line 4660 "lef.y"
    {
 if ((nonDefaultRuleName) && (*nonDefaultRuleName != '\0'))
 lefFree(nonDefaultRuleName);
 ;}
    break;

  case 428:
#line 4665 "lef.y"
    {
 if (strcmp(nonDefaultRuleName, (yyvsp[(2) - (2)].string)) != 0) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END NONDEFAULTRULE name %s is different from the NONDEFAULTRULE name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(2) - (2)].string), nonDefaultRuleName);
 lefError(1619, outMsg);
 lefFree(nonDefaultRuleName);
 lefFree(outMsg);
 CHKERR();
 } else
 lefFree(nonDefaultRuleName);
 } else
 lefFree(nonDefaultRuleName);
 } else
 lefFree(nonDefaultRuleName);
 ;}
    break;

  case 430:
#line 4691 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "HARDSPACING statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1620, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrNonDefaultCbk)
 lefiNonDefault_setHardspacing( &(lefrNonDefault) ) ;
 ;}
    break;

  case 440:
#line 4726 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "USEVIA statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1621, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addUseVia( &(lefrNonDefault),
 (yyvsp[(2) - (3)].string)) ;
 }
 ;}
    break;

  case 441:
#line 4745 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "USEVIARULE statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1622, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addUseViaRule( &(lefrNonDefault),
 (yyvsp[(2) - (3)].string)) ;
 }
 ;}
    break;

  case 442:
#line 4766 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "MINCUTS statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1623, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addMinCuts( &(lefrNonDefault),
 (yyvsp[(2) - (4)].string),
(int)(yyvsp[(3) - (4)].dval)) ;
 }
 ;}
    break;

  case 443:
#line 4787 "lef.y"
    { lefDumbMode = 10000000; lefRealNum = 1; lefInProp = 1; ;}
    break;

  case 444:
#line 4788 "lef.y"
    { lefDumbMode = 0;
 lefRealNum = 0;
 lefInProp = 0;
 ;}
    break;

  case 447:
#line 4800 "lef.y"
    {
 if (lefrNonDefaultCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrNondefProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiNonDefault_addProp( &(lefrNonDefault),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 448:
#line 4812 "lef.y"
    {
 if (lefrNonDefaultCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrNondefProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiNonDefault_addProp( &(lefrNonDefault),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 449:
#line 4824 "lef.y"
    {
 if (lefrNonDefaultCbk) {
 char temp[32];
 char propTp;
 sprintf(temp, "%.11g", (yyvsp[(2) - (2)].dval));
 propTp = lefiPropType_propType( &(lefrNondefProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiNonDefault_addNumProp( &(lefrNonDefault),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].dval),
temp,
propTp) ;
 }
 ;}
    break;

  case 450:
#line 4839 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 451:
#line 4840 "lef.y"
    {
 if (lefrNonDefaultCbk) lefiNonDefault_addLayer( &(lefrNonDefault),
 (yyvsp[(3) - (3)].string)) ;
 ndLayer++;
 /* $3);
  */
layerName = strdup((yyvsp[(3) - (3)].string));
 ndLayerWidth = 0;
 ndLayerSpace = 0;
 ;}
    break;

  case 452:
#line 4851 "lef.y"
    {
 ndLayerWidth = 1;
 if (lefrNonDefaultCbk) lefiNonDefault_addWidth( &(lefrNonDefault),
 (yyvsp[(6) - (7)].dval)) ;
 ;}
    break;

  case 453:
#line 4856 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 454:
#line 4857 "lef.y"
    {
 if (strcmp(layerName, (yyvsp[(12) - (12)].string)) != 0) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END LAYER name %s is different from the LAYER name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (12)].string), layerName);
 lefError(1624, outMsg);
 lefFree(outMsg);
 lefFree(layerName);
 CHKERR();
 } else
 lefFree(layerName);
 } else
 lefFree(layerName);
 } else
 lefFree(layerName);
 if (!ndLayerWidth) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 lefError(1625, "A WIDTH statement is required in the LAYER statement in NONDEFULTRULE.");
 CHKERR();
 }
 }
 }
 if (!ndLayerSpace && versionNum < 5.6) { /* 5.6, SPACING is optional
 */
if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "A SPACING statement is required in the LAYER statement in NONDEFAULTRULE for lef file with version 5.5 and earlier.\nYour lef file is defined with version %g. Update your lef to add a LAYER statement and try again.",
 versionNum);
 lefError(1626, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 ;}
    break;

  case 457:
#line 4910 "lef.y"
    {
 ndLayerSpace = 1;
 if (lefrNonDefaultCbk) lefiNonDefault_addSpacing( &(lefrNonDefault),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 458:
#line 4916 "lef.y"
    { if (lefrNonDefaultCbk)
 lefiNonDefault_addWireExtension( &(lefrNonDefault),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 459:
#line 4921 "lef.y"
    {
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addResistance( &(lefrNonDefault),
 (yyvsp[(3) - (4)].dval)) ;
 } else if (versionNum < 5.4) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "RESISTANCE RPERSQ statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1627, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (versionNum > 5.5) { /* obsolete in 5.6
 */
if (lefrNonDefaultCbk) /* write warning only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
 lefWarning(2029, "RESISTANCE RPERSQ statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 } else if (lefrNonDefaultCbk)
 lefiNonDefault_addResistance( &(lefrNonDefault),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 460:
#line 4950 "lef.y"
    {
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addCapacitance( &(lefrNonDefault),
 (yyvsp[(3) - (4)].dval)) ;
 } else if (versionNum < 5.4) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "CAPACITANCE CPERSQDIST statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1628, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (versionNum > 5.5) { /* obsolete in 5.6
 */
if (lefrNonDefaultCbk) /* write warning only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
 lefWarning(2030, "CAPACITANCE CPERSQDIST statement is obsolete in version 5.6. and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 } else if (lefrNonDefaultCbk)
 lefiNonDefault_addCapacitance( &(lefrNonDefault),
 (yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 461:
#line 4978 "lef.y"
    {
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addEdgeCap( &(lefrNonDefault),
 (yyvsp[(2) - (3)].dval)) ;
 } else if (versionNum < 5.4) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "EDGECAPACITANCE statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1629, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (versionNum > 5.5) { /* obsolete in 5.6
 */
if (lefrNonDefaultCbk) /* write warning only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
 lefWarning(2031, "EDGECAPACITANCE statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 } else if (lefrNonDefaultCbk)
 lefiNonDefault_addEdgeCap( &(lefrNonDefault),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 462:
#line 5006 "lef.y"
    {
 if (versionNum < 5.6) { /* 5.6 syntax
 */
if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "DIAGWIDTH statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1630, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addDiagWidth( &(lefrNonDefault),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 463:
#line 5028 "lef.y"
    {
 if (lefrSiteCbk)
 CALLBACK(lefrSiteCbk, lefrSiteCbkType, &lefrSite);
 ;}
    break;

  case 464:
#line 5033 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 465:
#line 5034 "lef.y"
    {
 if (lefrSiteCbk) lefiSite_setName( &(lefrSite),
 (yyvsp[(3) - (3)].string)) ;
 /* $3);
  */
siteName = strdup((yyvsp[(3) - (3)].string));
 hasSiteClass = 0;
 hasSiteSize = 0;
 hasSite = 1;
 ;}
    break;

  case 466:
#line 5045 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 467:
#line 5046 "lef.y"
    {
 if (strcmp(siteName, (yyvsp[(3) - (3)].string)) != 0) {
 if (lefrSiteCbk) { /* write error only if cbk is set */

 if (siteWarnings++ < lefrSiteWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END SITE name %s is different from the SITE name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (3)].string), siteName);
 lefError(1631, outMsg);
 lefFree(outMsg);
 lefFree(siteName);
 CHKERR();
 } else
 lefFree(siteName);
 } else
 lefFree(siteName);
 } else {
 lefFree(siteName);
 if (lefrSiteCbk) { /* write error only if cbk is set */

 if (hasSiteClass == 0) {
 lefError(1632, "A CLASS statement is required in the SITE statement.");
 CHKERR();
 }
 if (hasSiteSize == 0) {
 lefError(1633, "A SIZE statement is required in the SITE statement.");
 CHKERR();
 }
 }
 }
 ;}
    break;

  case 470:
#line 5086 "lef.y"
    {
 /* Workaround for pcr 640902
 */

 if (lefrSiteCbk) lefiSite_setSize( &(lefrSite),
 (yyvsp[(2) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 hasSiteSize = 1;
 ;}
    break;

  case 471:
#line 5096 "lef.y"
    { ;}
    break;

  case 472:
#line 5098 "lef.y"
    {
 if (lefrSiteCbk) lefiSite_setClass( &(lefrSite),
 (yyvsp[(1) - (1)].string)) ;
 hasSiteClass = 1;
 ;}
    break;

  case 473:
#line 5104 "lef.y"
    { ;}
    break;

  case 474:
#line 5107 "lef.y"
    {(yyval.string) = (char*)"PAD"; ;}
    break;

  case 475:
#line 5108 "lef.y"
    {(yyval.string) = (char*)"CORE"; ;}
    break;

  case 476:
#line 5109 "lef.y"
    {(yyval.string) = (char*)"VIRTUAL"; ;}
    break;

  case 477:
#line 5112 "lef.y"
    { ;}
    break;

  case 480:
#line 5122 "lef.y"
    { if (lefrSiteCbk) lefiSite_setXSymmetry( &(lefrSite) ) ;
 ;}
    break;

  case 481:
#line 5125 "lef.y"
    { if (lefrSiteCbk) lefiSite_setYSymmetry( &(lefrSite) ) ;
 ;}
    break;

  case 482:
#line 5128 "lef.y"
    { if (lefrSiteCbk) lefiSite_set90Symmetry( &(lefrSite) ) ;
 ;}
    break;

  case 483:
#line 5131 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 484:
#line 5133 "lef.y"
    { ;}
    break;

  case 487:
#line 5141 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 488:
#line 5142 "lef.y"
    { if (lefrSiteCbk) lefiSite_addRowPattern( &(lefrSite),
 (yyvsp[(1) - (3)].string),
(yyvsp[(2) - (3)].integer)) ;
 ;}
    break;

  case 489:
#line 5149 "lef.y"
    { (yyval.pt).x = (yyvsp[(1) - (2)].dval); (yyval.pt).y = (yyvsp[(2) - (2)].dval); ;}
    break;

  case 490:
#line 5151 "lef.y"
    { (yyval.pt).x = (yyvsp[(2) - (4)].dval); (yyval.pt).y = (yyvsp[(3) - (4)].dval); ;}
    break;

  case 491:
#line 5154 "lef.y"
    {
 if (lefrMacroCbk)
 CALLBACK(lefrMacroCbk, lefrMacroCbkType, &lefrMacro);
 lefrDoSite = 0;
 ;}
    break;

  case 493:
#line 5161 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 494:
#line 5162 "lef.y"
    {
 siteDef = 0;
 symDef = 0;
 sizeDef = 0;
 pinDef = 0;
 obsDef = 0;
 origDef = 0;
 lefiMacro_clear( &(lefrMacro) ) ;
 if (lefrMacroBeginCbk || lefrMacroCbk) {
 /* some reader may not have MacroBeginCB, but has MacroCB set
 */
lefiMacro_setName( &(lefrMacro),
 (yyvsp[(3) - (3)].string)) ;
 CALLBACK(lefrMacroBeginCbk, lefrMacroBeginCbkType, (yyvsp[(3) - (3)].string));
 }
 /* $3);
  */
macroName = strdup((yyvsp[(3) - (3)].string));
 ;}
    break;

  case 495:
#line 5182 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 496:
#line 5183 "lef.y"
    {
 if (strcmp(macroName, (yyvsp[(3) - (3)].string)) != 0) {
 if (lefrMacroEndCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END MACRO name %s is different from the MACRO name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (3)].string), macroName);
 lefError(1634, outMsg);
 lefFree(outMsg);
 lefFree(macroName);
 CHKERR();
 } else
 lefFree(macroName);
 } else
 lefFree(macroName);
 } else
 lefFree(macroName);
 if (lefrMacroEndCbk)
 CALLBACK(lefrMacroEndCbk, lefrMacroEndCbkType, (yyvsp[(3) - (3)].string));
 ;}
    break;

  case 504:
#line 5223 "lef.y"
    { ;}
    break;

  case 505:
#line 5225 "lef.y"
    { ;}
    break;

  case 506:
#line 5227 "lef.y"
    { ;}
    break;

  case 509:
#line 5231 "lef.y"
    { ;}
    break;

  case 510:
#line 5233 "lef.y"
    { ;}
    break;

  case 511:
#line 5235 "lef.y"
    { ;}
    break;

  case 512:
#line 5237 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setBuffer( &(lefrMacro) ) ;
 ;}
    break;

  case 513:
#line 5240 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setInverter( &(lefrMacro) ) ;
 ;}
    break;

  case 514:
#line 5243 "lef.y"
    { ;}
    break;

  case 515:
#line 5245 "lef.y"
    { ;}
    break;

  case 516:
#line 5247 "lef.y"
    { ;}
    break;

  case 517:
#line 5249 "lef.y"
    { ;}
    break;

  case 518:
#line 5250 "lef.y"
    {lefDumbMode = 1000000; lefRealNum = 1; lefInProp = 1; ;}
    break;

  case 519:
#line 5251 "lef.y"
    { lefDumbMode = 0;
 lefRealNum = 0;
 lefInProp = 0;
 ;}
    break;

  case 522:
#line 5262 "lef.y"
    {
 if (siteDef) { /* SITE is defined before SYMMETRY */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2032, "A SITE statement is defined before SYMMETRY statement.\nTo avoid this warning in the future, define SITE after SYMMETRY");
 }
 symDef = 1;
 ;}
    break;

  case 525:
#line 5283 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setXSymmetry( &(lefrMacro) ) ;
 ;}
    break;

  case 526:
#line 5286 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setYSymmetry( &(lefrMacro) ) ;
 ;}
    break;

  case 527:
#line 5289 "lef.y"
    { if (lefrMacroCbk) lefiMacro_set90Symmetry( &(lefrMacro) ) ;
 ;}
    break;

  case 528:
#line 5294 "lef.y"
    {
 char temp[32];
 sprintf(temp, "%.11g", (yyvsp[(2) - (2)].dval));
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiMacro_setNumProperty( &(lefrMacro),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].dval),
temp,
propTp) ;
 }
 ;}
    break;

  case 529:
#line 5309 "lef.y"
    {
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiMacro_setProperty( &(lefrMacro),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 530:
#line 5321 "lef.y"
    {
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiMacro_setProperty( &(lefrMacro),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 531:
#line 5334 "lef.y"
    {
 if (lefrMacroCbk) lefiMacro_setClass( &(lefrMacro),
 (yyvsp[(2) - (3)].string)) ;
 if (lefrMacroClassTypeCbk)
 CALLBACK(lefrMacroClassTypeCbk, lefrMacroClassTypeCbkType, (yyvsp[(2) - (3)].string));
 ;}
    break;

  case 532:
#line 5342 "lef.y"
    {(yyval.string) = (char*)"COVER"; ;}
    break;

  case 533:
#line 5344 "lef.y"
    { (yyval.string) = (char*)"COVER BUMP";
 if (versionNum < 5.5) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 if (lefrRelaxMode)
 lefWarning(2033, "The statement COVER BUMP is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
 else {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "COVER BUMP statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1635, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 ;}
    break;

  case 534:
#line 5363 "lef.y"
    {(yyval.string) = (char*)"RING"; ;}
    break;

  case 535:
#line 5364 "lef.y"
    {(yyval.string) = (char*)"BLOCK"; ;}
    break;

  case 536:
#line 5366 "lef.y"
    { (yyval.string) = (char*)"BLOCK BLACKBOX";
 if (versionNum < 5.5) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 if (lefrRelaxMode)
 lefWarning(2034, "The statement BLOCK BLACKBOX is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
 else {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "BLOCK BLACKBOX statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1636, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 ;}
    break;

  case 537:
#line 5386 "lef.y"
    {
 if (ignoreVersion) {
 (yyval.string) = (char*)"BLOCK SOFT";
 } else if (versionNum < 5.6) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "BLOCK SOFT statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1637, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 else
 (yyval.string) = (char*)"BLOCK SOFT";
 ;}
    break;

  case 538:
#line 5405 "lef.y"
    {(yyval.string) = (char*)"NONE"; ;}
    break;

  case 539:
#line 5408 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "BUMP is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1698, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else
 (yyval.string) = (char*)"BUMP";
 ;}
    break;

  case 540:
#line 5419 "lef.y"
    {(yyval.string) = (char*)"PAD"; ;}
    break;

  case 541:
#line 5420 "lef.y"
    {(yyval.string) = (char*)"VIRTUAL"; ;}
    break;

  case 542:
#line 5422 "lef.y"
    { sprintf(temp_name, "PAD %s", (yyvsp[(2) - (2)].string));
 (yyval.string) = temp_name;
 if (versionNum < 5.5) {
 if (strcmp("AREAIO", (yyvsp[(2) - (2)].string)) != 0) {
 sprintf(temp_name, "PAD %s", (yyvsp[(2) - (2)].string));
 (yyval.string) = temp_name;
 } else if (lefrMacroCbk) {
 if (macroWarnings++ < lefrMacroWarnings) {
 if (lefrRelaxMode)
 lefWarning(2035, "The statement PAD AREAIO is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
 else {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "PAD AREAIO statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1638, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 ;}
    break;

  case 543:
#line 5444 "lef.y"
    {(yyval.string) = (char*)"CORE"; ;}
    break;

  case 544:
#line 5446 "lef.y"
    {(yyval.string) = (char*)"CORNER";
 /* This token is NOT in the spec but has shown up in
 * some lef files. This exception came from LEFOUT
 * in 'frameworks'
 */

 ;}
    break;

  case 545:
#line 5454 "lef.y"
    {sprintf(temp_name, "CORE %s", (yyvsp[(2) - (2)].string));
 (yyval.string) = temp_name;;}
    break;

  case 546:
#line 5457 "lef.y"
    {sprintf(temp_name, "ENDCAP %s", (yyvsp[(2) - (2)].string));
 (yyval.string) = temp_name;;}
    break;

  case 547:
#line 5463 "lef.y"
    {(yyval.string) = (char*)"INPUT";;}
    break;

  case 548:
#line 5464 "lef.y"
    {(yyval.string) = (char*)"OUTPUT";;}
    break;

  case 549:
#line 5465 "lef.y"
    {(yyval.string) = (char*)"INOUT";;}
    break;

  case 550:
#line 5466 "lef.y"
    {(yyval.string) = (char*)"POWER";;}
    break;

  case 551:
#line 5467 "lef.y"
    {(yyval.string) = (char*)"SPACER";;}
    break;

  case 552:
#line 5468 "lef.y"
    {(yyval.string) = (char*)"AREAIO";;}
    break;

  case 553:
#line 5471 "lef.y"
    {(yyval.string) = (char*)"FEEDTHRU";;}
    break;

  case 554:
#line 5472 "lef.y"
    {(yyval.string) = (char*)"TIEHIGH";;}
    break;

  case 555:
#line 5473 "lef.y"
    {(yyval.string) = (char*)"TIELOW";;}
    break;

  case 556:
#line 5475 "lef.y"
    {
 if (ignoreVersion) {
 (yyval.string) = (char*)"SPACER";
 } else if (versionNum < 5.4) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SPACER statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1639, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 else
 (yyval.string) = (char*)"SPACER";
 ;}
    break;

  case 557:
#line 5495 "lef.y"
    {
 if (ignoreVersion) {
 (yyval.string) = (char*)"ANTENNACELL";
 } else if (versionNum < 5.4) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNACELL statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1640, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 else
 (yyval.string) = (char*)"ANTENNACELL";
 ;}
    break;

  case 558:
#line 5515 "lef.y"
    {
 if (ignoreVersion) {
 (yyval.string) = (char*)"WELLTAP";
 } else if (versionNum < 5.6) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "WELLTAP statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1641, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 else
 (yyval.string) = (char*)"WELLTAP";
 ;}
    break;

  case 559:
#line 5536 "lef.y"
    {(yyval.string) = (char*)"PRE";;}
    break;

  case 560:
#line 5537 "lef.y"
    {(yyval.string) = (char*)"POST";;}
    break;

  case 561:
#line 5538 "lef.y"
    {(yyval.string) = (char*)"TOPLEFT";;}
    break;

  case 562:
#line 5539 "lef.y"
    {(yyval.string) = (char*)"TOPRIGHT";;}
    break;

  case 563:
#line 5540 "lef.y"
    {(yyval.string) = (char*)"BOTTOMLEFT";;}
    break;

  case 564:
#line 5541 "lef.y"
    {(yyval.string) = (char*)"BOTTOMRIGHT";;}
    break;

  case 565:
#line 5544 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setGenerator( &(lefrMacro),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 566:
#line 5549 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setGenerate( &(lefrMacro),
 (yyvsp[(2) - (4)].string),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 567:
#line 5556 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "USER") ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2036, "SOURCE statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 568:
#line 5567 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "GENERATE") ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2037, "SOURCE statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 569:
#line 5578 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "BLOCK") ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2037, "SOURCE statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 570:
#line 5590 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrMacroCbk) lefiMacro_setPower( &(lefrMacro),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2038, "MACRO POWER statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 571:
#line 5602 "lef.y"
    {
 if (origDef) { /* Has multiple ORIGIN defined in a macro, stop parsing*/

 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1642, "ORIGIN statement has defined more than once in a MACRO statement.\nOnly one ORIGIN statement can be defined in a Macro.\nParser stops executions.");
 CHKERR();
 }
 }
 }
 origDef = 1;
 if (siteDef) { /* SITE is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2039, "A SITE statement is defined before ORIGIN statement.\nTo avoid this warning in the future, define SITE after ORIGIN");
 }
 if (pinDef) { /* PIN is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2040, "A PIN statement is defined before ORIGIN statement.\nTo avoid this warning in the future, define PIN after ORIGIN");
 }
 if (obsDef) { /* OBS is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2041, "A OBS statement is defined before ORIGIN statement.\nTo avoid this warning in the future, define OBS after ORIGIN");
 }
 /* 11/22/99 - Wanda da Rosa. PCR 283846
 can be defined any order.
 if (symDef) * SYMMETRY is defined before ORIGIN *
 lefWarning("SYMMETRY is defined before ORIGIN.");
 */

 /* Add back it back in per Nora request on PCR 283846 */

 /* 1/14/2000 - Wanda da Rosa, PCR 288770
 if (sizeDef) * SIZE is defined before ORIGIN *
 lefWarning("SIZE is defined before ORIGIN.");
 */

 
 /* Workaround for pcr 640902 */

 if (lefrMacroCbk) lefiMacro_setOrigin( &(lefrMacro),
 (yyvsp[(2) - (3)].pt).x,
(yyvsp[(2) - (3)].pt).y) ;
 if (lefrMacroOriginCbk) {
 macroNum.x = (yyvsp[(2) - (3)].pt).x;
 macroNum.y = (yyvsp[(2) - (3)].pt).y;
 CALLBACK(lefrMacroOriginCbk, lefrMacroOriginCbkType, macroNum);
 }
 ;}
    break;

  case 572:
#line 5669 "lef.y"
    { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 (yyvsp[(1) - (2)].string),
0,
0.0,
0.0,
-1) ;
 ;}
    break;

  case 573:
#line 5678 "lef.y"
    { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 (yyvsp[(1) - (3)].string),
1,
(yyvsp[(2) - (3)].pt).x,
(yyvsp[(2) - (3)].pt).y,
-1) ;
 ;}
    break;

  case 574:
#line 5687 "lef.y"
    { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 (yyvsp[(1) - (4)].string),
1,
(yyvsp[(2) - (4)].pt).x,
(yyvsp[(2) - (4)].pt).y,
(yyvsp[(3) - (4)].integer)) ;
 ;}
    break;

  case 575:
#line 5696 "lef.y"
    { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 (yyvsp[(1) - (3)].string),
0,
0.0,
0.0,
(yyvsp[(2) - (3)].integer)) ;
 ;}
    break;

  case 576:
#line 5705 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 577:
#line 5706 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setEEQ( &(lefrMacro),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 578:
#line 5710 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 579:
#line 5711 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setLEQ( &(lefrMacro),
 (yyvsp[(3) - (4)].string)) ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 lefWarning(2042, "LEQ statement in MACRO is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 580:
#line 5724 "lef.y"
    {
 if (lefrMacroCbk) {
 lefiMacro_setSiteName( &(lefrMacro),
 (yyvsp[(2) - (3)].string)) ;
 /* For later than 5.6 release
 lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
 sizeof(lefiSitePattern));
 lefiSitePattern_Init( lefrSitePatternPtr ) ;
 lefiSitePattern_setSiteName( lefrSitePatternPtr,
 $2) ;
 */

 }
 ;}
    break;

  case 581:
#line 5739 "lef.y"
    {
 if (lefrMacroCbk) {
 /* also set site name in the variable siteName_ in lefiMacro */

 /* this, if user wants to use method siteName will get the name also */

 /* Does not work, it will only set with the last sitename, if multiple
 SITEs are defined. Therefore, data will not be correct
 lefiMacro_setSitePatternName( &(lefrMacro),
 lefiSitePattern_name( lefrSitePatternPtr )) ;
 */

 lefiMacro_setSitePattern( &(	lefrMacro),
 lefrSitePatternPtr) ;
 	lefrSitePatternPtr = 0;
 }
 ;}
    break;

  case 582:
#line 5758 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; siteDef = 1;
 if (lefrMacroCbk) lefrDoSite = 1; ;}
    break;

  case 583:
#line 5762 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 584:
#line 5765 "lef.y"
    {
 if (siteDef) { /* SITE is defined before SIZE */

 /* pcr 283846 suppress warning
 if (siteWarnings++ < lefrSiteWarnings)
 lefWarning("SITE is defined before SIZE.");
 return 1;
 */

 }
 sizeDef = 1;
 if (lefrMacroCbk) lefiMacro_setSize( &(lefrMacro),
 (yyvsp[(2) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 if (lefrMacroSizeCbk) {
 macroNum.x = (yyvsp[(2) - (5)].dval);
 macroNum.y = (yyvsp[(4) - (5)].dval);
 CALLBACK(lefrMacroSizeCbk, lefrMacroSizeCbkType, macroNum);
 }
 ;}
    break;

  case 585:
#line 5791 "lef.y"
    {
 if (lefrPinCbk)
 CALLBACK(lefrPinCbk, lefrPinCbkType, &lefrPin);
 lefiPin_clear( &(lefrPin) ) ;
 ;}
    break;

  case 586:
#line 5797 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; pinDef = 1;;}
    break;

  case 587:
#line 5798 "lef.y"
    { if (lefrPinCbk) lefiPin_setName( &(lefrPin),
 (yyvsp[(3) - (3)].string)) ;
 /* $3);
  */
pinName = strdup((yyvsp[(3) - (3)].string));
 ;}
    break;

  case 588:
#line 5805 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 589:
#line 5806 "lef.y"
    {
 if (strcmp(pinName, (yyvsp[(3) - (3)].string)) != 0) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END PIN name %s is different from the PIN name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (3)].string), pinName);
 lefError(1643, outMsg);
 lefFree(outMsg);
 lefFree(pinName);
 CHKERR();
 } else
 lefFree(pinName);
 } else
 lefFree(pinName);
 } else
 lefFree(pinName);
 ;}
    break;

  case 590:
#line 5829 "lef.y"
    { ;}
    break;

  case 591:
#line 5831 "lef.y"
    { ;}
    break;

  case 592:
#line 5835 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 (yyvsp[(1) - (2)].string),
0,
0.0,
0.0,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2043, "FOREIGN statement in MACRO PIN is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 593:
#line 5850 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 (yyvsp[(1) - (3)].string),
1,
(yyvsp[(2) - (3)].pt).x,
(yyvsp[(2) - (3)].pt).y,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2043, "FOREIGN statement in MACRO PIN is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 594:
#line 5865 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 (yyvsp[(1) - (4)].string),
1,
(yyvsp[(2) - (4)].pt).x,
(yyvsp[(2) - (4)].pt).y,
(yyvsp[(3) - (4)].integer)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2043, "FOREIGN statement in MACRO PIN is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 595:
#line 5880 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 (yyvsp[(1) - (3)].string),
0,
0.0,
0.0,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2043, "FOREIGN statement in MACRO PIN is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 596:
#line 5895 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 (yyvsp[(1) - (4)].string),
1,
(yyvsp[(3) - (4)].pt).x,
(yyvsp[(3) - (4)].pt).y,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2043, "FOREIGN statement in MACRO PIN is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 597:
#line 5910 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 (yyvsp[(1) - (5)].string),
1,
(yyvsp[(3) - (5)].pt).x,
(yyvsp[(3) - (5)].pt).y,
(yyvsp[(4) - (5)].integer)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2043, "FOREIGN statement in MACRO PIN is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 598:
#line 5924 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 599:
#line 5925 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_setLEQ( &(lefrPin),
 (yyvsp[(3) - (4)].string)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2044, "LEQ statement in MACRO PIN is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 600:
#line 5936 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setPower( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2045, "MACRO POWER statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 601:
#line 5947 "lef.y"
    { if (lefrPinCbk) lefiPin_setDirection( &(lefrPin),
 (yyvsp[(1) - (1)].string)) ;
 ;}
    break;

  case 602:
#line 5951 "lef.y"
    { if (lefrPinCbk) lefiPin_setUse( &(lefrPin),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 603:
#line 5955 "lef.y"
    { ;}
    break;

  case 604:
#line 5957 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setLeakage( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2046, "MACRO LEAKAGE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, r emove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 605:
#line 5968 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseThresh( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2047, "MACRO RISETHRESH statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 606:
#line 5979 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallThresh( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2048, "MACRO FALLTHRESH statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 607:
#line 5990 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseSatcur( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2049, "MACRO RISESATCUR statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 608:
#line 6001 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallSatcur( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2050, "MACRO FALLSATCUR statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 609:
#line 6012 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setVLO( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2051, "MACRO VLO statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 610:
#line 6023 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setVHI( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2052, "MACRO VHI statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 611:
#line 6034 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setTieoffr( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2053, "MACRO TIEOFFR statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 612:
#line 6045 "lef.y"
    { if (lefrPinCbk) lefiPin_setShape( &(lefrPin),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 613:
#line 6048 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 614:
#line 6049 "lef.y"
    { if (lefrPinCbk) lefiPin_setMustjoin( &(lefrPin),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 615:
#line 6052 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 616:
#line 6053 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setOutMargin( &(lefrPin),
 (yyvsp[(3) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2054, "MACRO OUTPUTNOISEMARGIN statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 617:
#line 6064 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 618:
#line 6065 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setOutResistance( &(lefrPin),
 (yyvsp[(3) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2055, "MACRO OUTPUTRESISTANCE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 619:
#line 6076 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 620:
#line 6077 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setInMargin( &(lefrPin),
 (yyvsp[(3) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2056, "MACRO INPUTNOISEMARGIN statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 621:
#line 6089 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCapacitance( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2057, "MACRO CAPACITANCE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 622:
#line 6100 "lef.y"
    { if (lefrPinCbk) lefiPin_setMaxdelay( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 623:
#line 6104 "lef.y"
    { if (lefrPinCbk) lefiPin_setMaxload( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 624:
#line 6108 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setResistance( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2058, "MACRO RESISTANCE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 625:
#line 6119 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setPulldownres( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2059, "MACRO PULLDOWNRES statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 626:
#line 6130 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCurrentSource( &(lefrPin),
 "ACTIVE") ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2060, "MACRO CURRENTSOURCE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 627:
#line 6141 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCurrentSource( &(lefrPin),
 "RESISTIVE") ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2061, "MACRO CURRENTSOURCE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 628:
#line 6152 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseVoltage( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2062, "MACRO RISEVOLTAGETHRESHOLD statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 629:
#line 6163 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallVoltage( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2063, "MACRO FALLVOLTAGETHRESHOLD statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 630:
#line 6174 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setTables( &(lefrPin),
 (yyvsp[(2) - (4)].string),
(yyvsp[(3) - (4)].string)) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2064, "MACRO IV_TABLES statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 631:
#line 6186 "lef.y"
    { if (lefrPinCbk) lefiPin_setTaperRule( &(lefrPin),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 632:
#line 6189 "lef.y"
    {lefDumbMode = 1000000; lefRealNum = 1; lefInProp = 1; ;}
    break;

  case 633:
#line 6190 "lef.y"
    { lefDumbMode = 0;
 lefRealNum = 0;
 lefInProp = 0;
 ;}
    break;

  case 634:
#line 6195 "lef.y"
    {
 lefDumbMode = 0;
 hasGeoLayer = 0;
 if (lefrPinCbk) {
 lefiPin_addPort( &(	lefrPin),
 lefrGeometriesPtr) ;
 	lefrGeometriesPtr = 0;
 	lefrDoGeometries = 0;
 }
 if ((needGeometry) && (needGeometry != 2)) /* if the last LAYER in PORT
 */
if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 lefWarning(2065, "Either PATH, RECT or POLYGON statement is a required in MACRO/PIN/PORT.");
 ;}
    break;

  case 635:
#line 6216 "lef.y"
    {
 /* Since in start_macro_port it has call the Init method, here
 */
/* we need to call the Destroy method.
 */
/* Still add a null pointer to set the number of port
 */
if (lefrPinCbk) {
 lefiPin_addPort( &(lefrPin),
 lefrGeometriesPtr) ;
 lefrGeometriesPtr = 0;
 lefrDoGeometries = 0;
 }
 hasGeoLayer = 0;
 ;}
    break;

  case 636:
#line 6232 "lef.y"
    { /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNASIZE statement is a version 5.3 and earlier syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1644, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaSize( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 637:
#line 6258 "lef.y"
    { /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMETALAREA statement is a version 5.3 and earlier syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1645, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMetalArea( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 638:
#line 6284 "lef.y"
    { /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMETALLENGTH statement is a version 5.3 and earlier syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1646, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMetalLength( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 639:
#line 6310 "lef.y"
    { if (lefrPinCbk) lefiPin_setRiseSlewLimit( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 640:
#line 6314 "lef.y"
    { if (lefrPinCbk) lefiPin_setFallSlewLimit( &(lefrPin),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 641:
#line 6318 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAPARTIALMETALAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1647, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAPARTIALMETALAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1647, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialMetalArea( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 642:
#line 6354 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAPARTIALMETALSIDEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1648, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAPARTIALMETALSIDEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1648, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialMetalSideArea( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 643:
#line 6390 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAPARTIALCUTAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1649, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAPARTIALCUTAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1649, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialCutArea( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 644:
#line 6426 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNADIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1650, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNADIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1650, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaDiffArea( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 645:
#line 6462 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAGATEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1651, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAGATEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1651, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaGateArea( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 646:
#line 6498 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMAXAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1652, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMAXAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1652, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxAreaCar( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 647:
#line 6534 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMAXSIDEAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1653, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMAXSIDEAREACAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1653, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxSideAreaCar( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 648:
#line 6570 "lef.y"
    { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMAXCUTCAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1654, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMAXCUTCAR statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1654, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxCutCar( &(lefrPin),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 649:
#line 6606 "lef.y"
    { /* 5.5 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.5) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1655, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAMODEL statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1655, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 ;}
    break;

  case 651:
#line 6639 "lef.y"
    {lefDumbMode = 2; lefNoNum = 2; ;}
    break;

  case 652:
#line 6640 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "NETEXPR statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1656, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrPinCbk) lefiPin_setNetExpr( &(lefrPin),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 653:
#line 6657 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 654:
#line 6658 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "SUPPLYSENSITIVITY statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1657, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrPinCbk) lefiPin_setSupplySensitivity( &(lefrPin),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 655:
#line 6675 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 656:
#line 6676 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "GROUNDSENSITIVITY statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1658, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else
 if (lefrPinCbk) lefiPin_setGroundSensitivity( &(lefrPin),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 657:
#line 6696 "lef.y"
    {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 1) ;
 ;}
    break;

  case 658:
#line 6702 "lef.y"
    {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 2) ;
 ;}
    break;

  case 659:
#line 6708 "lef.y"
    {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 3) ;
 ;}
    break;

  case 660:
#line 6714 "lef.y"
    {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 4) ;
 ;}
    break;

  case 663:
#line 6727 "lef.y"
    {
 char temp[32];
 sprintf(temp, "%.11g", (yyvsp[(2) - (2)].dval));
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiPin_setNumProperty( &(lefrPin),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].dval),
temp,
propTp) ;
 }
 ;}
    break;

  case 664:
#line 6742 "lef.y"
    {
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiPin_setProperty( &(lefrPin),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 665:
#line 6754 "lef.y"
    {
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 (yyvsp[(1) - (2)].string)) ;
 lefiPin_setProperty( &(lefrPin),
 (yyvsp[(1) - (2)].string),
(yyvsp[(2) - (2)].string),
propTp) ;
 }
 ;}
    break;

  case 666:
#line 6767 "lef.y"
    {(yyval.string) = (char*)"INPUT";;}
    break;

  case 667:
#line 6768 "lef.y"
    {(yyval.string) = (char*)"OUTPUT";;}
    break;

  case 668:
#line 6769 "lef.y"
    {(yyval.string) = (char*)"OUTPUT TRISTATE";;}
    break;

  case 669:
#line 6770 "lef.y"
    {(yyval.string) = (char*)"INOUT";;}
    break;

  case 670:
#line 6771 "lef.y"
    {(yyval.string) = (char*)"FEEDTHRU";;}
    break;

  case 671:
#line 6774 "lef.y"
    {
 if (lefrPinCbk) {
 	lefrDoGeometries = 1;
 hasPRP = 0;
 	lefrGeometriesPtr = (lefiGeometries*)lefMalloc( sizeof(lefiGeometries));
 lefiGeometries_Init( 	lefrGeometriesPtr ) ;
 }
 needGeometry = 0; /* don't need rect/path/poly define yet
 */
hasGeoLayer = 0; /* make sure LAYER is set before geometry
 */
;}
    break;

  case 673:
#line 6790 "lef.y"
    { if (lefrDoGeometries)
 lefiGeometries_addClass( lefrGeometriesPtr,
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 674:
#line 6796 "lef.y"
    {(yyval.string) = (char*)"SIGNAL";;}
    break;

  case 675:
#line 6797 "lef.y"
    {(yyval.string) = (char*)"ANALOG";;}
    break;

  case 676:
#line 6798 "lef.y"
    {(yyval.string) = (char*)"POWER";;}
    break;

  case 677:
#line 6799 "lef.y"
    {(yyval.string) = (char*)"GROUND";;}
    break;

  case 678:
#line 6800 "lef.y"
    {(yyval.string) = (char*)"CLOCK";;}
    break;

  case 679:
#line 6801 "lef.y"
    {(yyval.string) = (char*)"DATA";;}
    break;

  case 680:
#line 6804 "lef.y"
    {(yyval.string) = (char*)"INPUT";;}
    break;

  case 681:
#line 6805 "lef.y"
    {(yyval.string) = (char*)"OUTPUT";;}
    break;

  case 682:
#line 6806 "lef.y"
    {(yyval.string) = (char*)"START";;}
    break;

  case 683:
#line 6807 "lef.y"
    {(yyval.string) = (char*)"STOP";;}
    break;

  case 684:
#line 6811 "lef.y"
    {(yyval.string) = (char*)""; ;}
    break;

  case 685:
#line 6813 "lef.y"
    {(yyval.string) = (char*)"ABUTMENT";;}
    break;

  case 686:
#line 6814 "lef.y"
    {(yyval.string) = (char*)"RING";;}
    break;

  case 687:
#line 6815 "lef.y"
    {(yyval.string) = (char*)"FEEDTHRU";;}
    break;

  case 689:
#line 6820 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 690:
#line 6821 "lef.y"
    {
 if ((needGeometry) && (needGeometry != 2)) /* 1 LAYER follow after another
 */
if (layerWarnings++ < lefrLayerWarnings) {
 /* geometries is called by MACRO/OBS & MACRO/PIN/PORT */

 if (obsDef)
 lefWarning(2076, "Either PATH, RECT or POLYGON statement is a required in MACRO/OBS.");
 else
 lefWarning(2065, "Either PATH, RECT or POLYGON statement is a required in MACRO/PIN/PORT.");
 }
 if (lefrDoGeometries)
 lefiGeometries_addLayer( lefrGeometriesPtr,
 (yyvsp[(3) - (3)].string)) ;
 needGeometry = 1; /* within LAYER it requires either path, rect, poly
 */
hasGeoLayer = 1;
 ;}
    break;

  case 692:
#line 6842 "lef.y"
    {
 if (lefrDoGeometries) {
 if (hasGeoLayer == 0) { /* LAYER statement is missing */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
 CHKERR();
 }
 } else
 lefiGeometries_addWidth( lefrGeometriesPtr,
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 693:
#line 6856 "lef.y"
    { if (lefrDoGeometries) {
 if (hasGeoLayer == 0) { /* LAYER statement is missing */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
 CHKERR();
 }
 } else
 lefiGeometries_addPath( lefrGeometriesPtr ) ;
 }
 hasPRP = 1;
 needGeometry = 2;
 ;}
    break;

  case 694:
#line 6870 "lef.y"
    { if (lefrDoGeometries) {
 if (hasGeoLayer == 0) { /* LAYER statement is missing */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
 CHKERR();
 }
 } else
 lefiGeometries_addPathIter( lefrGeometriesPtr ) ;
 }
 hasPRP = 1;
 needGeometry = 2;
 ;}
    break;

  case 695:
#line 6884 "lef.y"
    { if (lefrDoGeometries) {
 if (hasGeoLayer == 0) { /* LAYER statement is missing */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
 CHKERR();
 }
 } else
 lefiGeometries_addRect( lefrGeometriesPtr,
 (yyvsp[(2) - (4)].pt).x,
(yyvsp[(2) - (4)].pt).y,
(yyvsp[(3) - (4)].pt).x,
(yyvsp[(3) - (4)].pt).y) ;
 }
 needGeometry = 2;
 ;}
    break;

  case 696:
#line 6901 "lef.y"
    { if (lefrDoGeometries) {
 if (hasGeoLayer == 0) { /* LAYER statement is missing */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
 CHKERR();
 }
 } else
 lefiGeometries_addRectIter( lefrGeometriesPtr,
 (yyvsp[(3) - (6)].pt).x,
(yyvsp[(3) - (6)].pt).y,
(yyvsp[(4) - (6)].pt).x,
(yyvsp[(4) - (6)].pt).y) ;
 }
 needGeometry = 2;
 ;}
    break;

  case 697:
#line 6918 "lef.y"
    {
 if (lefrDoGeometries) {
 if (hasGeoLayer == 0) { /* LAYER statement is missing */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
 CHKERR();
 }
 } else
 lefiGeometries_addPolygon( lefrGeometriesPtr ) ;
 }
 hasPRP = 1;
 needGeometry = 2;
 ;}
    break;

  case 698:
#line 6933 "lef.y"
    { if (lefrDoGeometries) {
 if (hasGeoLayer == 0) { /* LAYER statement is missing */

 if (macroWarnings++ < lefrMacroWarnings) {
 lefError(1701, "A LAYER statement is missing in Geometry.\nLAYER is a required statement before any geometry can be defined.");
 CHKERR();
 }
 } else
 lefiGeometries_addPolygonIter( lefrGeometriesPtr ) ;
 }
 hasPRP = 1;
 needGeometry = 2;
 ;}
    break;

  case 699:
#line 6947 "lef.y"
    { ;}
    break;

  case 703:
#line 6957 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "EXCEPTPGNET is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1699, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrDoGeometries)
 lefiGeometries_addLayerExceptPgNet( lefrGeometriesPtr ) ;
 }
 ;}
    break;

  case 705:
#line 6974 "lef.y"
    { if (lefrDoGeometries) {
 if (zeroOrGt((yyvsp[(2) - (2)].dval)))
 lefiGeometries_addLayerMinSpacing( lefrGeometriesPtr,
 (yyvsp[(2) - (2)].dval)) ;
 else {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "THE SPACING statement has the value %g in MACRO OBS.\nValue has to be 0 or greater.", (yyvsp[(2) - (2)].dval));
 lefError(1659, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 ;}
    break;

  case 706:
#line 6989 "lef.y"
    { if (lefrDoGeometries) {
 if (zeroOrGt((yyvsp[(2) - (2)].dval)))
 lefiGeometries_addLayerRuleWidth( lefrGeometriesPtr,
 (yyvsp[(2) - (2)].dval)) ;
 else {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "THE DESIGNRULEWIDTH statement has the value %g in MACRO OBS.\nValue has to be 0 or greater.", (yyvsp[(2) - (2)].dval));
 lefError(1660, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 ;}
    break;

  case 707:
#line 7005 "lef.y"
    { if (lefrDoGeometries)
 lefiGeometries_startList( lefrGeometriesPtr,
 (yyvsp[(1) - (1)].pt).x,
(yyvsp[(1) - (1)].pt).y) ;
 ;}
    break;

  case 708:
#line 7012 "lef.y"
    { if (lefrDoGeometries)
 lefiGeometries_addToList( lefrGeometriesPtr,
 (yyvsp[(1) - (1)].pt).x,
(yyvsp[(1) - (1)].pt).y) ;
 ;}
    break;

  case 711:
#line 7029 "lef.y"
    {lefDumbMode = 1;;}
    break;

  case 712:
#line 7030 "lef.y"
    { if (lefrDoGeometries)
 lefiGeometries_addVia( lefrGeometriesPtr,
 (yyvsp[(2) - (5)].pt).x,
(yyvsp[(2) - (5)].pt).y,
(yyvsp[(4) - (5)].string)) ;
 ;}
    break;

  case 713:
#line 7036 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 714:
#line 7038 "lef.y"
    { if (lefrDoGeometries)
 lefiGeometries_addViaIter( lefrGeometriesPtr,
 (yyvsp[(3) - (7)].pt).x,
(yyvsp[(3) - (7)].pt).y,
(yyvsp[(5) - (7)].string)) ;
 ;}
    break;

  case 715:
#line 7047 "lef.y"
    { if (lefrDoGeometries)
 lefiGeometries_addStepPattern( lefrGeometriesPtr,
 (yyvsp[(2) - (7)].dval),
(yyvsp[(4) - (7)].dval),
(yyvsp[(6) - (7)].dval),
(yyvsp[(7) - (7)].dval)) ;
 ;}
    break;

  case 716:
#line 7057 "lef.y"
    {
 if (lefrDoSite) {
 	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
 				 sizeof(lefiSitePattern));
 lefiSitePattern_Init( 	lefrSitePatternPtr ) ;
 lefiSitePattern_set( 	lefrSitePatternPtr,
 (yyvsp[(1) - (11)].string),
(yyvsp[(2) - (11)].dval),
(yyvsp[(3) - (11)].dval),
(yyvsp[(4) - (11)].integer),
(yyvsp[(6) - (11)].dval),
(yyvsp[(8) - (11)].dval),
(yyvsp[(10) - (11)].dval),
(yyvsp[(11) - (11)].dval)) ;
 	}
 ;}
    break;

  case 717:
#line 7074 "lef.y"
    {
 if (lefrDoSite) {
 	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
 				 sizeof(lefiSitePattern));
 lefiSitePattern_Init( 	lefrSitePatternPtr ) ;
 lefiSitePattern_set( 	lefrSitePatternPtr,
 (yyvsp[(1) - (4)].string),
(yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval),
(yyvsp[(4) - (4)].integer),
-1,
-1,
-1,
-1) ;
 	}
 ;}
    break;

  case 718:
#line 7093 "lef.y"
    {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 				sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "X",
(yyvsp[(2) - (6)].dval),
(int)(yyvsp[(4) - (6)].dval),(yyvsp[(6) - (6)].dval)) ;
 }
 ;}
    break;

  case 719:
#line 7104 "lef.y"
    {lefDumbMode = 1000000000;;}
    break;

  case 720:
#line 7105 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 721:
#line 7107 "lef.y"
    {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "Y",
(yyvsp[(2) - (6)].dval),
(int)(yyvsp[(4) - (6)].dval),(yyvsp[(6) - (6)].dval)) ;
 }
 ;}
    break;

  case 722:
#line 7118 "lef.y"
    {lefDumbMode = 1000000000;;}
    break;

  case 723:
#line 7119 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 724:
#line 7121 "lef.y"
    {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "X",
(yyvsp[(2) - (6)].dval),
(int)(yyvsp[(4) - (6)].dval),(yyvsp[(6) - (6)].dval)) ;
 }
 ;}
    break;

  case 725:
#line 7133 "lef.y"
    {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "Y",
(yyvsp[(2) - (6)].dval),
(int)(yyvsp[(4) - (6)].dval),(yyvsp[(6) - (6)].dval)) ;
 }
 ;}
    break;

  case 728:
#line 7152 "lef.y"
    { if (lefrDoTrack) lefiTrackPattern_addLayer( lefrTrackPatternPtr,
 (yyvsp[(1) - (1)].string)) ;
 ;}
    break;

  case 729:
#line 7157 "lef.y"
    {
 if (lefrDoGcell) {
 	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
 sizeof(lefiGcellPattern));
 lefiGcellPattern_Init( 	lefrGcellPatternPtr ) ;
 lefiGcellPattern_set( 	lefrGcellPatternPtr,
 "X",
(yyvsp[(2) - (6)].dval),
(int)(yyvsp[(4) - (6)].dval),(yyvsp[(6) - (6)].dval)) ;
 }
 ;}
    break;

  case 730:
#line 7169 "lef.y"
    {
 if (lefrDoGcell) {
 	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
 sizeof(lefiGcellPattern));
 lefiGcellPattern_Init( 	lefrGcellPatternPtr ) ;
 lefiGcellPattern_set( 	lefrGcellPatternPtr,
 "Y",
(yyvsp[(2) - (6)].dval),
(int)(yyvsp[(4) - (6)].dval),(yyvsp[(6) - (6)].dval)) ;
 }
 ;}
    break;

  case 731:
#line 7182 "lef.y"
    {
 if (lefrObstructionCbk) {
 lefiObstruction_setGeometries( &(	lefrObstruction),
 lefrGeometriesPtr) ;
 	lefrGeometriesPtr = 0;
 	lefrDoGeometries = 0;
 CALLBACK(lefrObstructionCbk, lefrObstructionCbkType, &lefrObstruction);
 }
 lefDumbMode = 0;
 hasGeoLayer = 0; /* reset */

 ;}
    break;

  case 732:
#line 7203 "lef.y"
    {
 /* The pointer has malloced in start, need to free manually */

 if (lefrGeometriesPtr) {
 lefiGeometries_Destroy( lefrGeometriesPtr ) ;
 lefFree(lefrGeometriesPtr);
 lefrGeometriesPtr = 0;
 	 lefrDoGeometries = 0;
 }
 hasGeoLayer = 0;
 ;}
    break;

  case 733:
#line 7216 "lef.y"
    {
 obsDef = 1;
 if (lefrObstructionCbk) {
 	lefrDoGeometries = 1;
 	lefrGeometriesPtr = (lefiGeometries*)lefMalloc(
 	 sizeof(lefiGeometries));
 lefiGeometries_Init( 	lefrGeometriesPtr ) ;
 	}
 hasGeoLayer = 0;
 ;}
    break;

  case 734:
#line 7228 "lef.y"
    {
 if (versionNum < 5.6) {
 if (lefrDensityCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "DENSITY statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1661, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrDensityCbk) {
 CALLBACK(lefrDensityCbk, lefrDensityCbkType, &lefrDensity);
 lefiDensity_clear( &(lefrDensity) ) ;
 }
 lefDumbMode = 0;
 ;}
    break;

  case 737:
#line 7254 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 738:
#line 7255 "lef.y"
    {
 if (lefrDensityCbk)
 lefiDensity_addLayer( &(lefrDensity),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 742:
#line 7268 "lef.y"
    {
 if (lefrDensityCbk)
 lefiDensity_addRect( &(lefrDensity),
 (yyvsp[(2) - (5)].pt).x,
(yyvsp[(2) - (5)].pt).y,
(yyvsp[(3) - (5)].pt).x,
(yyvsp[(3) - (5)].pt).y,
(yyvsp[(4) - (5)].dval)) ;
 ;}
    break;

  case 743:
#line 7278 "lef.y"
    { lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 744:
#line 7279 "lef.y"
    { if (lefrMacroCbk) lefiMacro_setClockType( &(lefrMacro),
 (yyvsp[(3) - (4)].string)) ;
 ;}
    break;

  case 745:
#line 7284 "lef.y"
    { ;}
    break;

  case 746:
#line 7287 "lef.y"
    { /* XXXXX for macros */
;}
    break;

  case 747:
#line 7291 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrTimingCbk && lefiTiming_hasData( &(lefrTiming) ) )  CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming) ;
 lefiTiming_clear( &(lefrTiming) ) ;
 } else {
 if (lefrTimingCbk) /* write warning only if cbk is set */

 if (timingWarnings++ < lefrTimingWarnings)
 lefWarning(2066, "MACRO TIMING statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 lefiTiming_clear( &(lefrTiming) ) ;
 }
 ;}
    break;

  case 750:
#line 7312 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrTimingCbk && lefiTiming_hasData( &(lefrTiming) ) )  CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming) ;
 }
 lefDumbMode = 1000000000;
 lefiTiming_clear( &(lefrTiming) ) ;
 ;}
    break;

  case 751:
#line 7320 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 752:
#line 7321 "lef.y"
    {lefDumbMode = 1000000000;;}
    break;

  case 753:
#line 7322 "lef.y"
    { lefDumbMode = 0;;}
    break;

  case 754:
#line 7324 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addRiseFall( &(lefrTiming),
 (yyvsp[(1) - (4)].string),
(yyvsp[(3) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 755:
#line 7330 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addRiseFallVariable( &(lefrTiming),
 (yyvsp[(8) - (10)].dval),
(yyvsp[(9) - (10)].dval)) ;
 ;}
    break;

  case 756:
#line 7336 "lef.y"
    { if (lefrTimingCbk) {
 	if ((yyvsp[(2) - (9)].string)[0] == 'D' || (yyvsp[(2) - (9)].string)[0] == 'd') /* delay */

 	 lefiTiming_addDelay( &(lefrTiming),
 (yyvsp[(1) - (9)].string),
(yyvsp[(4) - (9)].string),
(yyvsp[(6) - (9)].dval),
(yyvsp[(7) - (9)].dval),
(yyvsp[(8) - (9)].dval)) ;
 	else
 	 lefiTiming_addTransition( &(lefrTiming),
 (yyvsp[(1) - (9)].string),
(yyvsp[(4) - (9)].string),
(yyvsp[(6) - (9)].dval),
(yyvsp[(7) - (9)].dval),
(yyvsp[(8) - (9)].dval)) ;
 }
 ;}
    break;

  case 757:
#line 7355 "lef.y"
    { ;}
    break;

  case 758:
#line 7357 "lef.y"
    { ;}
    break;

  case 759:
#line 7359 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setRiseRS( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 760:
#line 7364 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setFallRS( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 761:
#line 7369 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setRiseCS( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 762:
#line 7374 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setFallCS( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 763:
#line 7379 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setRiseAtt1( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 764:
#line 7384 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setFallAtt1( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 765:
#line 7389 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setRiseTo( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 766:
#line 7394 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setFallTo( &(lefrTiming),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval)) ;
 ;}
    break;

  case 767:
#line 7399 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addUnateness( &(lefrTiming),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 768:
#line 7403 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setStable( &(lefrTiming),
 (yyvsp[(3) - (7)].dval),
(yyvsp[(5) - (7)].dval),
(yyvsp[(6) - (7)].string)) ;
 ;}
    break;

  case 769:
#line 7409 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addSDF2Pins( &(lefrTiming),
 (yyvsp[(1) - (8)].string),
(yyvsp[(2) - (8)].string),
(yyvsp[(3) - (8)].string),
(yyvsp[(5) - (8)].dval),
(yyvsp[(6) - (8)].dval),
(yyvsp[(7) - (8)].dval)) ;
 ;}
    break;

  case 770:
#line 7418 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addSDF1Pin( &(lefrTiming),
 (yyvsp[(1) - (6)].string),
(yyvsp[(3) - (6)].dval),
(yyvsp[(4) - (6)].dval),
(yyvsp[(4) - (6)].dval)) ;
 ;}
    break;

  case 771:
#line 7425 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setSDFcondStart( &(lefrTiming),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 772:
#line 7429 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setSDFcondEnd( &(lefrTiming),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 773:
#line 7433 "lef.y"
    { if (lefrTimingCbk) lefiTiming_setSDFcond( &(lefrTiming),
 (yyvsp[(2) - (3)].string)) ;
 ;}
    break;

  case 774:
#line 7437 "lef.y"
    { /* XXXXX */
;}
    break;

  case 775:
#line 7442 "lef.y"
    { (yyval.string) = (char*)"MPWH";;}
    break;

  case 776:
#line 7444 "lef.y"
    { (yyval.string) = (char*)"MPWL";;}
    break;

  case 777:
#line 7446 "lef.y"
    { (yyval.string) = (char*)"PERIOD";;}
    break;

  case 778:
#line 7450 "lef.y"
    { (yyval.string) = (char*)"SETUP";;}
    break;

  case 779:
#line 7452 "lef.y"
    { (yyval.string) = (char*)"HOLD";;}
    break;

  case 780:
#line 7454 "lef.y"
    { (yyval.string) = (char*)"RECOVERY";;}
    break;

  case 781:
#line 7456 "lef.y"
    { (yyval.string) = (char*)"SKEW";;}
    break;

  case 782:
#line 7460 "lef.y"
    { (yyval.string) = (char*)"ANYEDGE";;}
    break;

  case 783:
#line 7462 "lef.y"
    { (yyval.string) = (char*)"POSEDGE";;}
    break;

  case 784:
#line 7464 "lef.y"
    { (yyval.string) = (char*)"NEGEDGE";;}
    break;

  case 785:
#line 7468 "lef.y"
    { (yyval.string) = (char*)"ANYEDGE";;}
    break;

  case 786:
#line 7470 "lef.y"
    { (yyval.string) = (char*)"POSEDGE";;}
    break;

  case 787:
#line 7472 "lef.y"
    { (yyval.string) = (char*)"NEGEDGE";;}
    break;

  case 788:
#line 7476 "lef.y"
    { (yyval.string) = (char*)"DELAY"; ;}
    break;

  case 789:
#line 7478 "lef.y"
    { (yyval.string) = (char*)"TRANSITION"; ;}
    break;

  case 790:
#line 7482 "lef.y"
    { ;}
    break;

  case 791:
#line 7484 "lef.y"
    { ;}
    break;

  case 792:
#line 7487 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addTableEntry( &(lefrTiming),
 (yyvsp[(2) - (5)].dval),
(yyvsp[(3) - (5)].dval),
(yyvsp[(4) - (5)].dval)) ;
 ;}
    break;

  case 793:
#line 7495 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addTableAxisNumber( &(lefrTiming),
 (yyvsp[(1) - (1)].dval)) ;
 ;}
    break;

  case 794:
#line 7499 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addTableAxisNumber( &(lefrTiming),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 795:
#line 7506 "lef.y"
    { ;}
    break;

  case 796:
#line 7508 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addRiseFallSlew( &(lefrTiming),
 (yyvsp[(1) - (4)].dval),
(yyvsp[(2) - (4)].dval),
(yyvsp[(3) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 797:
#line 7515 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addRiseFallSlew( &(lefrTiming),
 (yyvsp[(1) - (7)].dval),
(yyvsp[(2) - (7)].dval),
(yyvsp[(3) - (7)].dval),
(yyvsp[(4) - (7)].dval)) ;
 if (lefrTimingCbk) lefiTiming_addRiseFallSlew2( &(lefrTiming),
 (yyvsp[(5) - (7)].dval),
(yyvsp[(6) - (7)].dval),
(yyvsp[(7) - (7)].dval)) ;
 ;}
    break;

  case 798:
#line 7528 "lef.y"
    { (yyval.string) = (char*)"RISE"; ;}
    break;

  case 799:
#line 7530 "lef.y"
    { (yyval.string) = (char*)"FALL"; ;}
    break;

  case 800:
#line 7534 "lef.y"
    { (yyval.string) = (char*)"INVERT"; ;}
    break;

  case 801:
#line 7536 "lef.y"
    { (yyval.string) = (char*)"NONINVERT"; ;}
    break;

  case 802:
#line 7538 "lef.y"
    { (yyval.string) = (char*)"NONUNATE"; ;}
    break;

  case 803:
#line 7542 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addFromPin( &(lefrTiming),
 (yyvsp[(1) - (1)].string)) ;
 ;}
    break;

  case 804:
#line 7546 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addFromPin( &(lefrTiming),
 (yyvsp[(2) - (2)].string)) ;
 ;}
    break;

  case 805:
#line 7552 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addToPin( &(lefrTiming),
 (yyvsp[(1) - (1)].string)) ;
 ;}
    break;

  case 806:
#line 7556 "lef.y"
    { if (lefrTimingCbk) lefiTiming_addToPin( &(lefrTiming),
 (yyvsp[(2) - (2)].string)) ;
 ;}
    break;

  case 807:
#line 7561 "lef.y"
    {
 if (lefrArrayCbk)
 	CALLBACK(lefrArrayCbk, lefrArrayCbkType, &lefrArray);
 lefiArray_clear( &(lefrArray) ) ;
 lefrSitePatternPtr = 0;
 lefrDoSite = 0;
 ;}
    break;

  case 809:
#line 7570 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 810:
#line 7571 "lef.y"
    {
 if (lefrArrayCbk) {
 lefiArray_setName( &(	lefrArray),
 (yyvsp[(3) - (3)].string)) ;
 	CALLBACK(lefrArrayBeginCbk, lefrArrayBeginCbkType, (yyvsp[(3) - (3)].string));
 }
 /* $3);
  */
arrayName = strdup((yyvsp[(3) - (3)].string));
 ;}
    break;

  case 811:
#line 7582 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1;;}
    break;

  case 812:
#line 7583 "lef.y"
    {
 if (lefrArrayCbk && lefrArrayEndCbk)
 	CALLBACK(lefrArrayEndCbk, lefrArrayEndCbkType, (yyvsp[(3) - (3)].string));
 if (strcmp(arrayName, (yyvsp[(3) - (3)].string)) != 0) {
 if (lefrArrayCbk) { /* write error only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "END ARRAY name %s is different from the ARRAY name %s.\nCorrect the LEF file before rerun it through the LEF parser.", (yyvsp[(3) - (3)].string), arrayName);
 lefError(1662, outMsg);
 lefFree(outMsg);
 lefFree(arrayName);
 CHKERR();
 } else
 lefFree(arrayName);
 } else
 lefFree(arrayName);
 } else
 lefFree(arrayName);
 ;}
    break;

  case 813:
#line 7608 "lef.y"
    { ;}
    break;

  case 814:
#line 7610 "lef.y"
    { ;}
    break;

  case 815:
#line 7613 "lef.y"
    { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;}
    break;

  case 816:
#line 7615 "lef.y"
    {
 if (lefrArrayCbk) {
 lefiArray_addSitePattern( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 ;}
    break;

  case 817:
#line 7621 "lef.y"
    {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;}
    break;

  case 818:
#line 7623 "lef.y"
    {
 if (lefrArrayCbk) {
 lefiArray_addCanPlace( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 ;}
    break;

  case 819:
#line 7629 "lef.y"
    {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;}
    break;

  case 820:
#line 7631 "lef.y"
    {
 if (lefrArrayCbk) {
 lefiArray_addCannotOccupy( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 ;}
    break;

  case 821:
#line 7637 "lef.y"
    { if (lefrArrayCbk) lefrDoTrack = 1; ;}
    break;

  case 822:
#line 7638 "lef.y"
    {
 if (lefrArrayCbk) {
 lefiArray_addTrack( &(	lefrArray),
 lefrTrackPatternPtr) ;
 }
 ;}
    break;

  case 823:
#line 7645 "lef.y"
    {
 ;}
    break;

  case 824:
#line 7647 "lef.y"
    { if (lefrArrayCbk) lefrDoGcell = 1; ;}
    break;

  case 825:
#line 7648 "lef.y"
    {
 if (lefrArrayCbk) {
 lefiArray_addGcell( &(	lefrArray),
 lefrGcellPatternPtr) ;
 }
 ;}
    break;

  case 826:
#line 7655 "lef.y"
    {
 if (lefrArrayCbk) {
 lefiArray_setTableSize( &(	lefrArray),
 (int)(yyvsp[(2) - (5)].dval)) ;
 }
 ;}
    break;

  case 827:
#line 7662 "lef.y"
    { ;}
    break;

  case 828:
#line 7665 "lef.y"
    { if (lefrArrayCbk) lefiArray_addFloorPlan( &(lefrArray),
 (yyvsp[(2) - (2)].string)) ;
 ;}
    break;

  case 829:
#line 7672 "lef.y"
    { ;}
    break;

  case 830:
#line 7674 "lef.y"
    { ;}
    break;

  case 831:
#line 7677 "lef.y"
    { lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; ;}
    break;

  case 832:
#line 7679 "lef.y"
    {
 if (lefrArrayCbk)
 lefiArray_addSiteToFloorPlan( &(	lefrArray),
 "CANPLACE",
	lefrSitePatternPtr) ;
 ;}
    break;

  case 833:
#line 7685 "lef.y"
    { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; ;}
    break;

  case 834:
#line 7687 "lef.y"
    {
 if (lefrArrayCbk)
 lefiArray_addSiteToFloorPlan( &(	lefrArray),
 "CANNOTOCCUPY",
	lefrSitePatternPtr) ;
 ;}
    break;

  case 835:
#line 7697 "lef.y"
    { ;}
    break;

  case 836:
#line 7699 "lef.y"
    { ;}
    break;

  case 837:
#line 7702 "lef.y"
    { if (lefrArrayCbk) lefiArray_addDefaultCap( &(lefrArray),
 (int)(yyvsp[(2) - (5)].dval),(yyvsp[(4) - (5)].dval)) ;
 ;}
    break;

  case 838:
#line 7707 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 839:
#line 7708 "lef.y"
    { lefAddStringMessage((yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].string)); ;}
    break;

  case 840:
#line 7711 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 841:
#line 7712 "lef.y"
    { ;}
    break;

  case 842:
#line 7715 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 843:
#line 7716 "lef.y"
    {
 if (versionNum < 5.6)
 lefAddNumDefine((yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].dval));
 else
 if (lefrArrayCbk) /* write warning only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings)
 lefWarning(2067, "DEFINE statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 844:
#line 7725 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 845:
#line 7726 "lef.y"
    {
 if (versionNum < 5.6)
 lefAddStringDefine((yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].string));
 else
 if (lefrArrayCbk) /* write warning only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings)
 lefWarning(2068, "DEFINES statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 846:
#line 7735 "lef.y"
    {lefDumbMode=1;lefNlToken=TRUE;;}
    break;

  case 847:
#line 7736 "lef.y"
    {
 if (versionNum < 5.6)
 lefAddBooleanDefine((yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].integer));
 else
 if (lefrArrayCbk) /* write warning only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings)
 lefWarning(2069, "DEFINEB statement is obsolete in version 5.6 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.6 or later.");
 ;}
    break;

  case 849:
#line 7749 "lef.y"
    {lefNlToken = FALSE;;}
    break;

  case 850:
#line 7750 "lef.y"
    {lefNlToken = FALSE;;}
    break;

  case 855:
#line 7763 "lef.y"
    {(yyval.dval) = (yyvsp[(1) - (3)].dval) + (yyvsp[(3) - (3)].dval); ;}
    break;

  case 856:
#line 7764 "lef.y"
    {(yyval.dval) = (yyvsp[(1) - (3)].dval) - (yyvsp[(3) - (3)].dval); ;}
    break;

  case 857:
#line 7765 "lef.y"
    {(yyval.dval) = (yyvsp[(1) - (3)].dval) * (yyvsp[(3) - (3)].dval); ;}
    break;

  case 858:
#line 7766 "lef.y"
    {(yyval.dval) = (yyvsp[(1) - (3)].dval) / (yyvsp[(3) - (3)].dval); ;}
    break;

  case 859:
#line 7767 "lef.y"
    {(yyval.dval) = -(yyvsp[(2) - (2)].dval);;}
    break;

  case 860:
#line 7768 "lef.y"
    {(yyval.dval) = (yyvsp[(2) - (3)].dval);;}
    break;

  case 861:
#line 7770 "lef.y"
    {(yyval.dval) = ((yyvsp[(2) - (6)].integer) != 0) ? (yyvsp[(4) - (6)].dval) : (yyvsp[(6) - (6)].dval);;}
    break;

  case 862:
#line 7771 "lef.y"
    {(yyval.dval) = (yyvsp[(1) - (1)].dval);;}
    break;

  case 863:
#line 7774 "lef.y"
    {(yyval.integer) = comp_num((yyvsp[(1) - (3)].dval),(yyvsp[(2) - (3)].integer),(yyvsp[(3) - (3)].dval));;}
    break;

  case 864:
#line 7775 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].dval) != 0 && (yyvsp[(3) - (3)].dval) != 0;;}
    break;

  case 865:
#line 7776 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].dval) != 0 || (yyvsp[(3) - (3)].dval) != 0;;}
    break;

  case 866:
#line 7777 "lef.y"
    {(yyval.integer) = comp_str((yyvsp[(1) - (3)].string),(yyvsp[(2) - (3)].integer),(yyvsp[(3) - (3)].string));;}
    break;

  case 867:
#line 7778 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].string)[0] != 0 && (yyvsp[(3) - (3)].string)[0] != 0;;}
    break;

  case 868:
#line 7779 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].string)[0] != 0 || (yyvsp[(3) - (3)].string)[0] != 0;;}
    break;

  case 869:
#line 7780 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].integer) == (yyvsp[(3) - (3)].integer);;}
    break;

  case 870:
#line 7781 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].integer) != (yyvsp[(3) - (3)].integer);;}
    break;

  case 871:
#line 7782 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].integer) && (yyvsp[(3) - (3)].integer);;}
    break;

  case 872:
#line 7783 "lef.y"
    {(yyval.integer) = (yyvsp[(1) - (3)].integer) || (yyvsp[(3) - (3)].integer);;}
    break;

  case 873:
#line 7784 "lef.y"
    {(yyval.integer) = !(yyval.integer);;}
    break;

  case 874:
#line 7785 "lef.y"
    {(yyval.integer) = (yyvsp[(2) - (3)].integer);;}
    break;

  case 875:
#line 7787 "lef.y"
    {(yyval.integer) = ((yyvsp[(2) - (6)].integer) != 0) ? (yyvsp[(4) - (6)].integer) : (yyvsp[(6) - (6)].integer);;}
    break;

  case 876:
#line 7788 "lef.y"
    {(yyval.integer) = 1;;}
    break;

  case 877:
#line 7789 "lef.y"
    {(yyval.integer) = 0;;}
    break;

  case 878:
#line 7793 "lef.y"
    {
 (yyval.string) = (char*)lefMalloc(strlen((yyvsp[(1) - (3)].string))+strlen((yyvsp[(3) - (3)].string))+1);
 strcpy((yyval.string),(yyvsp[(1) - (3)].string));
 strcat((yyval.string),(yyvsp[(3) - (3)].string));
 ;}
    break;

  case 879:
#line 7799 "lef.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 880:
#line 7801 "lef.y"
    {
 lefDefIf = TRUE;
 if ((yyvsp[(2) - (6)].integer) != 0) {
 	(yyval.string) = (yyvsp[(4) - (6)].string);
 } else {
 	(yyval.string) = (yyvsp[(6) - (6)].string);
 }
 ;}
    break;

  case 881:
#line 7810 "lef.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); ;}
    break;

  case 882:
#line 7813 "lef.y"
    {(yyval.integer) = C_LE;;}
    break;

  case 883:
#line 7814 "lef.y"
    {(yyval.integer) = C_LT;;}
    break;

  case 884:
#line 7815 "lef.y"
    {(yyval.integer) = C_GE;;}
    break;

  case 885:
#line 7816 "lef.y"
    {(yyval.integer) = C_GT;;}
    break;

  case 886:
#line 7817 "lef.y"
    {(yyval.integer) = C_EQ;;}
    break;

  case 887:
#line 7818 "lef.y"
    {(yyval.integer) = C_NE;;}
    break;

  case 888:
#line 7819 "lef.y"
    {(yyval.integer) = C_EQ;;}
    break;

  case 889:
#line 7820 "lef.y"
    {(yyval.integer) = C_LT;;}
    break;

  case 890:
#line 7821 "lef.y"
    {(yyval.integer) = C_GT;;}
    break;

  case 891:
#line 7825 "lef.y"
    {
 if (lefrPropBeginCbk)
 CALLBACK(lefrPropBeginCbk, lefrPropBeginCbkType, 0);
 lefInPropDef = 1; /* set flag as inside propertydefinitions */

 ;}
    break;

  case 892:
#line 7832 "lef.y"
    {
 if (lefrPropEndCbk)
 CALLBACK(lefrPropEndCbk, lefrPropEndCbkType, 0);
 lefRealNum = 0; /* just want to make sure it is reset */

 lefInPropDef = 0; /* reset flag */

 ;}
    break;

  case 893:
#line 7844 "lef.y"
    { ;}
    break;

  case 894:
#line 7846 "lef.y"
    { ;}
    break;

  case 895:
#line 7849 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 896:
#line 7852 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "library",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrLibProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 ;}
    break;

  case 897:
#line 7863 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 898:
#line 7866 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "componentpin",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrCompProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 ;}
    break;

  case 899:
#line 7877 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 900:
#line 7880 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "pin",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrPinProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 
 ;}
    break;

  case 901:
#line 7892 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 902:
#line 7895 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "macro",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrMacroProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 ;}
    break;

  case 903:
#line 7906 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 904:
#line 7909 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "via",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrViaProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 ;}
    break;

  case 905:
#line 7920 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 906:
#line 7923 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "viarule",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrViaRuleProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 ;}
    break;

  case 907:
#line 7934 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 908:
#line 7937 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "layer",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrLayerProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 ;}
    break;

  case 909:
#line 7948 "lef.y"
    {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 ;}
    break;

  case 910:
#line 7951 "lef.y"
    {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "nondefaultrule",
(yyvsp[(3) - (5)].string)) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrNondefProp),
 (yyvsp[(3) - (5)].string),
lefPropDefType) ;
 ;}
    break;

  case 911:
#line 7964 "lef.y"
    { lefRealNum = 0 ;}
    break;

  case 912:
#line 7965 "lef.y"
    {
 if (lefrPropCbk) lefiProp_setPropInteger( &(lefrProp) ) ;
 lefPropDefType = 'I'
 ;}
    break;

  case 913:
#line 7969 "lef.y"
    { lefRealNum = 1 ;}
    break;

  case 914:
#line 7970 "lef.y"
    {
 if (lefrPropCbk) lefiProp_setPropReal( &(lefrProp) ) ;
 lefPropDefType = 'R';
 lefRealNum = 0;
 ;}
    break;

  case 915:
#line 7976 "lef.y"
    {
 if (lefrPropCbk) lefiProp_setPropString( &(lefrProp) ) ;
 lefPropDefType = 'S'
 ;}
    break;

  case 916:
#line 7981 "lef.y"
    {
 if (lefrPropCbk) lefiProp_setPropQString( &(lefrProp),
 (yyvsp[(2) - (2)].string)) ;
 lefPropDefType = 'Q'
 ;}
    break;

  case 917:
#line 7987 "lef.y"
    {
 if (lefrPropCbk) lefiProp_setPropNameMapString( &(lefrProp),
 (yyvsp[(2) - (2)].string)) ;
 lefPropDefType = 'S'
 ;}
    break;

  case 918:
#line 7996 "lef.y"
    { ;}
    break;

  case 919:
#line 7998 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingRangeUseLength( &(lefrLayer) ) ;
 ;}
    break;

  case 920:
#line 8003 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingRangeInfluence( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 lefiLayer_setSpacingRangeInfluenceRange( &(lefrLayer),
 -1,
-1) ;
 }
 ;}
    break;

  case 921:
#line 8013 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingRangeInfluence( &(lefrLayer),
 (yyvsp[(2) - (5)].dval)) ;
 lefiLayer_setSpacingRangeInfluenceRange( &(lefrLayer),
 (yyvsp[(4) - (5)].dval),
(yyvsp[(5) - (5)].dval)) ;
 }
 ;}
    break;

  case 922:
#line 8023 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingRangeRange( &(lefrLayer),
 (yyvsp[(2) - (3)].dval),
(yyvsp[(3) - (3)].dval)) ;
 ;}
    break;

  case 923:
#line 8034 "lef.y"
    { ;}
    break;

  case 924:
#line 8036 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingParSW( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 926:
#line 8048 "lef.y"
    { ;}
    break;

  case 927:
#line 8050 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingParTwoEdges( &(lefrLayer) ) ;
 ;}
    break;

  case 928:
#line 8059 "lef.y"
    { ;}
    break;

  case 929:
#line 8061 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingSamenetPGonly( &(lefrLayer) ) ;
 ;}
    break;

  case 930:
#line 8069 "lef.y"
    { ;}
    break;

  case 931:
#line 8071 "lef.y"
    { if (lefrPropCbk) lefiProp_setRange( &(lefrProp),
 (yyvsp[(2) - (3)].dval),
(yyvsp[(3) - (3)].dval)) ;
 ;}
    break;

  case 932:
#line 8079 "lef.y"
    { ;}
    break;

  case 933:
#line 8081 "lef.y"
    { if (lefrPropCbk) lefiProp_setNumber( &(lefrProp),
 (yyvsp[(1) - (1)].dval)) ;
 ;}
    break;

  case 936:
#line 8092 "lef.y"
    {
 if (lefrLayerCbk) {
 if (hasSpCenter) {
 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1663, "A CENTERTOCENTER statement has already defined in SPACING\nCENTERTOCENTER can only be defined once per LAYER CUT SPACING.");
 CHKERR();
 }
 }
 hasSpCenter = 1;
 if (versionNum < 5.6) {
 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "CENTERTOCENTER statement is a version 5.6 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1664, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setSpacingCenterToCenter( &(lefrLayer) ) ;
 }
 ;}
    break;

  case 937:
#line 8117 "lef.y"
    {
 if (lefrLayerCbk) {
 if (hasSpSamenet) {
 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1665, "A SAMENET statement has already defined in SPACING\nSAMENET can only be defined once per LAYER CUT SPACING.");
 CHKERR();
 }
 }
 hasSpSamenet = 1;
 if (lefrLayerCbk)
 lefiLayer_setSpacingSamenet( &(lefrLayer) ) ;
 }
 ;}
    break;

  case 938:
#line 8131 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "SAMENET is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1684, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 ;}
    break;

  case 939:
#line 8143 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "PARALLELOVERLAP is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1680, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk) {
 if (hasSpParallel) {
 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1666, "A PARALLELOVERLAP statement has already defined in SPACING\nPARALLELOVERLAP can only be defined once per LAYER CUT SPACING.");
 CHKERR();
 }
 }
 hasSpParallel = 1;
 if (lefrLayerCbk)
 lefiLayer_setSpacingParallelOverlap( &(lefrLayer) ) ;
 }
 }
 ;}
    break;

  case 941:
#line 8169 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 942:
#line 8170 "lef.y"
    {
 if (lefrLayerCbk)
 {
 if (versionNum < 5.7) {
 if (hasSpSamenet) { /* 5.6 and earlier does not allow */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1667, "A SAMENET statement has already defined in SPACING\nEither SAMENET or LAYER can be defined, but not both.");
 CHKERR();
 }
 }
 }
 lefiLayer_setSpacingName( &(lefrLayer),
 (yyvsp[(3) - (3)].string)) ;
 }
 ;}
    break;

  case 944:
#line 8188 "lef.y"
    {
 if (lefrLayerCbk) {
 if (versionNum < 5.5) {
 if (layerWarnings++ < lefrLayerWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ADJACENTCUTS statement is a version 5.5 and later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1668, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 if (versionNum < 5.7) {
 if (hasSpSamenet) { /* 5.6 and earlier does not allow */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1669, "A SAMENET statement has already defined in SPACING\nEither SAMENET or ADJACENTCUTS can be defined, but not both.");
 CHKERR();
 }
 }
 }
 lefiLayer_setSpacingAdjacent( &(lefrLayer),
 (int)(yyvsp[(2) - (4)].dval),(yyvsp[(4) - (4)].dval)) ;
 }
 ;}
    break;

  case 946:
#line 8216 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "AREA is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1693, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk) {
 if (versionNum < 5.7) {
 if (hasSpSamenet) { /* 5.6 and earlier does not allow */

 if (layerWarnings++ < lefrLayerWarnings) {
 lefError(1670, "A SAMENET statement has already defined in SPACING\nEither SAMENET or AREA can be defined, but not both.");
 CHKERR();
 }
 }
 }
 lefiLayer_setSpacingArea( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 }
 ;}
    break;

  case 947:
#line 8241 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingRange( &(lefrLayer),
 (yyvsp[(2) - (3)].dval),
(yyvsp[(3) - (3)].dval)) ;
 ;}
    break;

  case 949:
#line 8249 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingLength( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 ;}
    break;

  case 950:
#line 8256 "lef.y"
    {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingLength( &(lefrLayer),
 (yyvsp[(2) - (5)].dval)) ;
 lefiLayer_setSpacingLengthRange( &(lefrLayer),
 (yyvsp[(4) - (5)].dval),
(yyvsp[(5) - (5)].dval)) ;
 }
 ;}
    break;

  case 951:
#line 8267 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingEol( &(lefrLayer),
 (yyvsp[(2) - (4)].dval),
(yyvsp[(4) - (4)].dval)) ;
 ;}
    break;

  case 952:
#line 8274 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "ENDOFLINE is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1681, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 ;}
    break;

  case 953:
#line 8286 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "NOTCHLENGTH is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1682, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk)
 lefiLayer_setSpacingNotchLength( &(lefrLayer),
 (yyvsp[(2) - (2)].dval)) ;
 }
 ;}
    break;

  case 954:
#line 8301 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "ENDOFNOTCHWIDTH is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1696, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk)
 lefiLayer_setSpacingEndOfNotchWidth( &(lefrLayer),
 (yyvsp[(2) - (6)].dval),
(yyvsp[(4) - (6)].dval),
(yyvsp[(6) - (6)].dval)) ;
 }
 ;}
    break;

  case 955:
#line 8322 "lef.y"
    {;}
    break;

  case 956:
#line 8324 "lef.y"
    {
 if (lefrLayerCbk)
 lefiLayer_setSpacingLayerStack( &(lefrLayer) ) ;
 ;}
    break;

  case 957:
#line 8333 "lef.y"
    {;}
    break;

  case 958:
#line 8335 "lef.y"
    {
 if (versionNum < 5.7) {
 outMsg = (char*)lefMalloc(10000);
 sprintf(outMsg,
 "EXCEPTSAMEPGNET is a version 5.7 or later syntax.\nYour lef file is defined with version %g.", versionNum);
 lefError(1683, outMsg);
 lefFree(outMsg);
 CHKERR();
 } else {
 if (lefrLayerCbk)
 lefiLayer_setSpacingAdjacentExcept( &(lefrLayer) ) ;
 }
 ;}
    break;

  case 959:
#line 8352 "lef.y"
    { (yyval.string) = 0; ;}
    break;

  case 960:
#line 8353 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 961:
#line 8354 "lef.y"
    { (yyval.string) = (yyvsp[(3) - (3)].string); ;}
    break;

  case 962:
#line 8359 "lef.y"
    {lefDumbMode = 1; lefNoNum = 1; ;}
    break;

  case 963:
#line 8360 "lef.y"
    { (yyval.string) = (yyvsp[(3) - (3)].string); ;}
    break;

  case 964:
#line 8365 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrNoiseMarginCbk) {
 lefrNoiseMargin.low = (yyvsp[(2) - (4)].dval);
 lefrNoiseMargin.high = (yyvsp[(3) - (4)].dval);
 CALLBACK(lefrNoiseMarginCbk, lefrNoiseMarginCbkType, &lefrNoiseMargin);
 }
 } else
 if (lefrNoiseMarginCbk) /* write warning only if cbk is set */

 if (noiseMarginWarnings++ < lefrNoiseMarginWarnings)
 lefWarning(2070, "UNIVERSALNOISEMARGIN statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 965:
#line 8380 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrEdgeRateThreshold1Cbk) {
 CALLBACK(lefrEdgeRateThreshold1Cbk,
 lefrEdgeRateThreshold1CbkType, (yyvsp[(2) - (3)].dval));
 }
 } else
 if (lefrEdgeRateThreshold1Cbk) /* write warning only if cbk is set */

 if (edgeRateThreshold1Warnings++ < lefrEdgeRateThreshold1Warnings)
 lefWarning(2071, "EDGERATETHRESHOLD1 statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 966:
#line 8394 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrEdgeRateThreshold2Cbk) {
 	 CALLBACK(lefrEdgeRateThreshold2Cbk,
 	 lefrEdgeRateThreshold2CbkType, (yyvsp[(2) - (3)].dval));
 }
 } else
 if (lefrEdgeRateThreshold2Cbk) /* write warning only if cbk is set */

 if (edgeRateThreshold2Warnings++ < lefrEdgeRateThreshold2Warnings)
 lefWarning(2072, "EDGERATETHRESHOLD2 statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 967:
#line 8408 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrEdgeRateScaleFactorCbk) {
 	 CALLBACK(lefrEdgeRateScaleFactorCbk,
 	 lefrEdgeRateScaleFactorCbkType, (yyvsp[(2) - (3)].dval));
 }
 } else
 if (lefrEdgeRateScaleFactorCbk) /* write warning only if cbk is set */

 if (edgeRateScaleFactorWarnings++ < lefrEdgeRateScaleFactorWarnings)
 lefWarning(2073, "EDGERATESCALEFACTOR statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 968:
#line 8422 "lef.y"
    { if (lefrNoiseTableCbk) lefiNoiseTable_setup( &(lefrNoiseTable),
 (int)(yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 969:
#line 8426 "lef.y"
    { ;}
    break;

  case 970:
#line 8430 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrNoiseTableCbk)
 CALLBACK(lefrNoiseTableCbk, lefrNoiseTableCbkType, &lefrNoiseTable);
 } else
 if (lefrNoiseTableCbk) /* write warning only if cbk is set */

 if (noiseTableWarnings++ < lefrNoiseTableWarnings)
 lefWarning(2074, "NOISETABLE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 973:
#line 8449 "lef.y"
    { if (lefrNoiseTableCbk)
 {
 lefiNoiseTable_newEdge( &(lefrNoiseTable) ) ;
 lefiNoiseTable_addEdge( &(lefrNoiseTable),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 974:
#line 8457 "lef.y"
    { ;}
    break;

  case 975:
#line 8460 "lef.y"
    { if (lefrNoiseTableCbk) lefiNoiseTable_addResistance( &(lefrNoiseTable) ) ;
 ;}
    break;

  case 977:
#line 8467 "lef.y"
    { if (lefrNoiseTableCbk)
 lefiNoiseTable_addResistanceNumber( &(lefrNoiseTable),
 (yyvsp[(1) - (1)].dval)) ;
 ;}
    break;

  case 978:
#line 8472 "lef.y"
    { if (lefrNoiseTableCbk)
 lefiNoiseTable_addResistanceNumber( &(lefrNoiseTable),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 981:
#line 8483 "lef.y"
    { if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimLength( &(	lefrNoiseTable),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 982:
#line 8488 "lef.y"
    { ;}
    break;

  case 983:
#line 8492 "lef.y"
    { if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimNoise( &(lefrNoiseTable),
 (yyvsp[(1) - (1)].dval)) ;
 ;}
    break;

  case 984:
#line 8497 "lef.y"
    { if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimNoise( &(lefrNoiseTable),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 985:
#line 8503 "lef.y"
    { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_setup( &(lefrCorrectionTable),
 (int)(yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 986:
#line 8508 "lef.y"
    { ;}
    break;

  case 987:
#line 8512 "lef.y"
    {
 if (versionNum < 5.4) {
 if (lefrCorrectionTableCbk)
 CALLBACK(lefrCorrectionTableCbk, lefrCorrectionTableCbkType,
 &lefrCorrectionTable);
 } else
 if (lefrCorrectionTableCbk) /* write warning only if cbk is set */

 if (correctionTableWarnings++ < lefrCorrectionTableWarnings)
 lefWarning(2075, "CORRECTIONTABLE statement is obsolete in version 5.4 and later.\nThe LEF parser will ignore this statement.\nTo avoid this warning in the future, remove this statement from the LEF file with version 5.4 or later.");
 ;}
    break;

  case 990:
#line 8531 "lef.y"
    { if (lefrCorrectionTableCbk)
 {
 lefiCorrectionTable_newEdge( &(lefrCorrectionTable) ) ;
 lefiCorrectionTable_addEdge( &(lefrCorrectionTable),
 (yyvsp[(2) - (3)].dval)) ;
 }
 ;}
    break;

  case 991:
#line 8539 "lef.y"
    { ;}
    break;

  case 992:
#line 8542 "lef.y"
    { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistance( &(lefrCorrectionTable) ) ;
 ;}
    break;

  case 993:
#line 8546 "lef.y"
    { ;}
    break;

  case 994:
#line 8550 "lef.y"
    { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistanceNumber( &(lefrCorrectionTable),
 (yyvsp[(1) - (1)].dval)) ;
 ;}
    break;

  case 995:
#line 8555 "lef.y"
    { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistanceNumber( &(lefrCorrectionTable),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 998:
#line 8567 "lef.y"
    { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimLength( &(lefrCorrectionTable),
 (yyvsp[(2) - (3)].dval)) ;
 ;}
    break;

  case 999:
#line 8572 "lef.y"
    { ;}
    break;

  case 1000:
#line 8576 "lef.y"
    { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimCorrection( &(	lefrCorrectionTable),
 (yyvsp[(1) - (1)].dval)) ;
 ;}
    break;

  case 1001:
#line 8581 "lef.y"
    { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimCorrection( &(	lefrCorrectionTable),
 (yyvsp[(2) - (2)].dval)) ;
 ;}
    break;

  case 1002:
#line 8590 "lef.y"
    { /* 5.3 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum > 5.3) {
 /* A 5.3 syntax in 5.4 */

 if (use5_4) {
 if (lefrInputAntennaCbk) { /* write warning only if cbk is set */

 if (inputAntennaWarnings++ < lefrInputAntennaWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "INPUTPINANTENNASIZE statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and INPUTPINANTENNASIZE syntax, which is illegal.", versionNum) ;
 lefError(1671, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 if (lefrInputAntennaCbk)
 CALLBACK(lefrInputAntennaCbk, lefrInputAntennaCbkType, (yyvsp[(2) - (3)].dval));
 ;}
    break;

  case 1003:
#line 8618 "lef.y"
    { /* 5.3 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum > 5.3) {
 /* A 5.3 syntax in 5.4 */

 if (use5_4) {
 if (lefrOutputAntennaCbk) { /* write warning only if cbk is set */

 if (outputAntennaWarnings++ < lefrOutputAntennaWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "OUTPUTPINANTENNASIZE statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and OUTPUTPINANTENNASIZE syntax, which is illegal.", versionNum) ;
 lefError(1672, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 if (lefrOutputAntennaCbk)
 CALLBACK(lefrOutputAntennaCbk, lefrOutputAntennaCbkType, (yyvsp[(2) - (3)].dval));
 ;}
    break;

  case 1004:
#line 8646 "lef.y"
    { /* 5.3 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum > 5.3) {
 /* A 5.3 syntax in 5.4 */

 if (use5_4) {
 if (lefrInoutAntennaCbk) { /* write warning only if cbk is set */

 if (inoutAntennaWarnings++ < lefrInoutAntennaWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "INOUTPINANTENNASIZE statement is a version 5.3 or earlier syntax.\nYour lef file with version %g, has both old and INOUTPINANTENNASIZE syntax, which is illegal.", versionNum) ;
 lefError(1673, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 }
 if (lefrInoutAntennaCbk)
 CALLBACK(lefrInoutAntennaCbk, lefrInoutAntennaCbkType, (yyvsp[(2) - (3)].dval));
 ;}
    break;

  case 1005:
#line 8674 "lef.y"
    { /* 5.4 syntax */

 /* 11/12/2002 - this is obsolete in 5.5, suppose should be ingored */

 /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrAntennaInputCbk) { /* write warning only if cbk is set */

 if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAINPUTGATEAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.\nEither update your VERSION number or use the 5.3 syntax.", versionNum);
 lefError(1674, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrAntennaInputCbk) { /* write warning only if cbk is set */

 if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAINPUTGATEAREA statement is a version 5.4 or later syntax.\nYour lef file with version %g, has both old and ANTENNAINPUTGATEAREA syntax, which is illegal.", versionNum) ;
 lefError(1675, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrAntennaInputCbk)
 CALLBACK(lefrAntennaInputCbk, lefrAntennaInputCbkType, (yyvsp[(2) - (3)].dval));
 ;}
    break;

  case 1006:
#line 8714 "lef.y"
    { /* 5.4 syntax */

 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */

 if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAINOUTDIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.\nEither update your VERSION number or use the 5.3 syntax.", versionNum);
 lefError(1676, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */

 if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAINOUTDIFFAREA statement is a version 5.4 or later syntax.\nYour lef file with version %g, has both old and ANTENNAINOUTDIFFAREA syntax, which is illegal.", versionNum) ;
 lefError(1677, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrAntennaInoutCbk)
 CALLBACK(lefrAntennaInoutCbk, lefrAntennaInoutCbkType, (yyvsp[(2) - (3)].dval));
 ;}
    break;

  case 1007:
#line 8754 "lef.y"
    { /* 5.4 syntax */

 /* 11/12/2002 - this is obsolete in 5.5, & will be ignored */

 /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */

 if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAOUTPUTDIFFAREA statement is a version 5.4 and later syntax.\nYour lef file is defined with version %g.\nEither update your VERSION number or use the 5.3 syntax.", versionNum);
 lefError(1678, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */

 if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "ANTENNAOUTPUTDIFFAREA statement is a version 5.4 or later syntax.\nYour lef file with version %g, has both old and ANTENNAOUTPUTDIFFAREA syntax, which is illegal.", versionNum) ;
 lefError(1679, outMsg);
 lefFree(outMsg);
 CHKERR();
 }
 }
 }
 if (lefrAntennaOutputCbk)
 CALLBACK(lefrAntennaOutputCbk, lefrAntennaOutputCbkType, (yyvsp[(2) - (3)].dval));
 ;}
    break;

  case 1010:
#line 8798 "lef.y"
    {
 if (lefrExtensionCbk)
 CALLBACK(lefrExtensionCbk, lefrExtensionCbkType, Hist_text);
 if (versionNum >= 5.6)
 ge56almostDone = 1;
 ;}
    break;


/* Line 1267 of yacc.c.  */
#line 14707 "lef.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 8805 "lef.y"

 

