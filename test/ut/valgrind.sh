valgrind -v --tool=memcheck --num-callers=15 --track-origins=yes --gen-suppressions=no --leak-check=full --leak-resolution=high --show-reachable=yes --xml=yes --xml-file=log.xml ./jvalut
