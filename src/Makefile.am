

EXTRA_DIST = parser.y scanner.l

bin_PROGRAMS = chomik

lib_LTLIBRARIES = libchomik.la


libchomik_la_SOURCES = lex.chomik_scanner.c chomik_parser.tab.c parser_functions.cc scanner_functions.cc report.cc make_copy_with_replacements.cc \
		execute.cc get_actual_code_value.cc get_result_replacing_placeholders.cc various_functions.cc generators.cc language_specific_code.cc bridge.cc


chomik_SOURCES = chomik.cc

chomik_LDFLAGS = @chomik_linker_flags@
chomik_LDADD = libchomik.la

AM_CXXFLAGS = @CXXFLAGS@ -fconcepts -std=c++17 -I.. -I../inc @chomik_compiler_flags@

TESTS = ../tests/test1.chomik ../tests/test2.chomik ../tests/test3.chomik ../tests/test4.chomik ../tests/test5.chomik ../tests/test6.chomik \
	../tests/test7.chomik ../tests/test8.chomik ../tests/test9.chomik ../tests/test10.chomik ../tests/test11.chomik ../tests/test12.chomik \
	../tests/test13.chomik ../tests/test14.chomik ../tests/test15.chomik ../tests/test16.chomik ../tests/test17.chomik ../tests/test18.chomik \
        ../tests/test19.chomik ../tests/test20.chomik ../tests/test21.chomik ../tests/test22.chomik ../tests/test23.chomik ../tests/test24.chomik \
        ../tests/test25.chomik ../tests/test26.chomik ../tests/test27.chomik ../tests/test28.chomik ../tests/test29.sh ../tests/test30.chomik \
        ../tests/test31.chomik ../tests/test32.chomik ../tests/test33.chomik ../tests/test34.chomik ../tests/test35.chomik ../tests/test36.chomik \
        ../tests/test37.chomik ../tests/test38.chomik ../tests/test39.chomik ../tests/test40.chomik ../tests/test41.chomik ../tests/test42.chomik \
        ../tests/test43.chomik ../tests/test44.chomik ../tests/test45.chomik ../tests/test46.chomik ../tests/test47.chomik ../tests/test48.chomik \
        ../tests/test49.chomik ../tests/test50.chomik ../tests/test51.chomik ../tests/test52.chomik ../tests/test53.chomik ../tests/test54.chomik \
        ../tests/test55.chomik ../tests/test56.chomik ../tests/test57.chomik ../tests/test58.sh ../tests/test59.sh ../tests/test60.sh ../tests/test61.sh

XFAIL_TESTS = ../tests/test3.chomik ../tests/test6.chomik ../tests/test8.chomik ../tests/test59.sh


# here we use a new cool chomik feature (version 3.0.1) - the command line option --set-the-chomik-string (there are chomik string variables 1 to 10)
# and the --execute command line option - this is useful when we want to change the directory
# also - we use here a new predefined code: 'change directory (DIR:string)'.

# However I had to give up the idea because it does not work well with XFAIL_TESTS. Instead the test29.chomik got a wrapper called test29.sh

#TESTS_ENVIRONMENT = $(top_builddir)/src/chomik --set-the-chomik-string 1 "$(top_builddir)/tests" --set-the-chomik-string 2 "$$f" --set-the-chomik-string 3 "$(top_builddir)/src" --execute "let the print target stream index = value integer 2; <print \"Running\" <the chomik \"string\" 2>>;"

lex.chomik_scanner.c: scanner.l chomik_parser.tab.h
	flex -Pchomik_scanner $<

scanner.c: lex.chomik_scanner.c
	cp $^ $@

scanner.lo: chomik_parser.tab.h

chomik_parser.tab.hh: parser.y
	bison --language="C++" --file-prefix="chomik_parser" -Dapi.prefix={chomik_parser} -d $^

chomik_parser.tab.h: parser.y
	bison --language="C" --file-prefix="chomik_parser" -Dapi.prefix={chomik_parser} -d $^

chomik_parser.tab.c: chomik_parser.tab.h

clean-local:
	rm -f chomik_parser.tab.h chomik_parser.tab.c chomik_parser.tab.hh chomik_parser.tab.cc lex.chomik_scanner.c location.hh position.hh stack.hh parser.c
	
print_linker_flags:
	@echo @chomik_linker_flags@

just_a_test:
	echo $(TEST_ENVIRONMENT)
