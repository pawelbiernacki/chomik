

bin_PROGRAMS = chomik

lib_LTLIBRARIES = libchomik.la


libchomik_la_SOURCES = scanner.l parser.y parser_functions.cc scanner_functions.cc report.cc make_copy_with_replacements.cc execute.cc get_actual_code_value.cc \
		get_result_replacing_placeholders.cc various_functions.cc generators.cc


chomik_SOURCES = chomik.cc

chomik_LDFLAGS = @chomik_linker_flags@
chomik_LDADD = libchomik.la

AM_CXXFLAGS = @CXXFLAGS@ -fconcepts -std=c++17 -I.. -I../inc @chomik_compiler_flags@



TESTS = ../tests/test1.chomik ../tests/test2.chomik ../tests/test3.chomik ../tests/test4.chomik ../tests/test5.chomik ../tests/test6.chomik \
	../tests/test7.chomik ../tests/test8.chomik ../tests/test9.chomik ../tests/test10.chomik ../tests/test11.chomik ../tests/test12.chomik \
	../tests/test13.chomik ../tests/test14.chomik ../tests/test15.chomik ../tests/test16.chomik ../tests/test17.chomik ../tests/test18.chomik

XFAIL_TESTS = ../tests/test3.chomik ../tests/test6.chomik ../tests/test8.chomik

		
lex.yy.c: scanner.l
	flex $^
	
scanner.c: lex.yy.c
	cp $^ $@

scanner.lo: parser.tab.h

parser.tab.h: parser.y
	bison -d $^

clean-local:
	rm -f parser.tab.h parser.tab.c lex.yy.c
	
print_linker_flags:
	@echo @chomik_linker_flags@
