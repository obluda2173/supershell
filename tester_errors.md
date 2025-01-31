
# Table of Contents

1.  [./tester](#orgd4f9216)
2.  [./tester builtins](#orgd2a24f2)
3.  [./tester redirects](#org2a2fe6b)
    1.  [ok](#org1b6e0d6)
    2.  [to fix](#org6f52d3c)
4.  [tester extras](#orgd39b241)
5.  [tester os<sub>specific</sub>](#org13d2966)
6.  [./tester bonus](#org62a4e50)
7.  [./tester wildcards](#orgc17f56d)

r Errors


<a id="orgd4f9216"></a>

# ./tester

OK<sub>TESTS</sub>:
Test  30: ❌ export HELLO-=123
Test  31: ❌ export =

FAILING<sub>TESTS</sub>:
export HELLO=123 A
export ECHO=echo
then
$ECHO

e&ldquo;c&rdquo;h"o


<a id="orgd2a24f2"></a>

# ./tester builtins

-   Test  26: ❌ export A-
    -   mini exit code = 0
    -   bash exit code = 1
-   Test  30: ❌ export HELLO-=123
    -   mini exit code = 0
    -   bash exit code = 1
-   Test  31: ❌ export =
    -   mini exit code = 0
    -   bash exit code = 1
-   Test  32: ❌ export 123
    -   mini exit code = 0
    -   bash exit code = 1


<a id="org2a2fe6b"></a>

# ./tester redirects


<a id="org1b6e0d6"></a>

## ok

-   Test  68: ✅⚠️  echo <&ldquo;./test<sub>files</sub>/infile<sub>big</sub>&rdquo; | echo <&ldquo;./test<sub>files</sub>/infile&rdquo;
-   mini error = ()
-   bash error = ( Broken pipe)
-   Test  71: ✅⚠️  cat <&ldquo;./test<sub>files</sub>/infile&rdquo; | echo hi
-   mini error = ()
-   bash error = ( Broken pipe)


<a id="org6f52d3c"></a>

## to fix

-   Test  73: ❌ cat <&ldquo;./test<sub>files</sub>/infile&rdquo; | echo hi
    -   mini exit code = 1
    -   bash exit code = 0
-   Test  75: ❌ cat <&ldquo;./test<sub>files</sub>/infile<sub>big</sub>&rdquo; | echo hi
    -   mini exit code = 1
    -   bash exit code = 0


<a id="orgd39b241"></a>

# tester extras

-   Test   2: ❌ $PWD
    -   mini exit code = 1
    -   bash exit code = 126
    -   mini error = ( Permission denied)
    -   bash error = ( Is a directory)
-   Test   3: ❌ $EMPTY
    -   mini exit code = 1
    -   bash exit code = 0
    -   mini error = ( Permission denied)
    -   bash error = ()
-   Test   4: ❌ $EMPTY echo hi
    -   mini output = ()
    -   bash output = (hi)
    -   mini exit code = 1
    -   bash exit code = 0
    -   mini error = ( Permission denied)
    -   bash error = ()
-   Test   5: ❌ ./test<sub>files</sub>/invalid<sub>permission</sub>
    -   mini exit code = 1
    -   bash exit code = 126
-   Test  10: ❌ ./test<sub>files</sub>
    -   mini exit code = 1
    -   bash exit code = 126
    -   mini error = ( Permission denied)
    -   bash error = ( Is a directory)

10/15
😭 😭 😭


<a id="org13d2966"></a>

# tester os<sub>specific</sub>

-   Test   8: ❌ exit 9223372036854775808
    -   mini exit code = 0
    -   bash exit code = 2
    -   mini error = ()
    -   bash error = ( numeric argument required)
-   Test  11: ❌ exit -9223372036854775809
    -   mini exit code = 255
    -   bash exit code = 2
    -   mini error = ()
    -   bash error = ( numeric argument required)
-   Test   3: ❌ export HELLO=123 A- WORLD=456
    -   mini exit code = 0
    -   bash exit code = 1
-   Test   5: ❌ export UNO=1 DOS-2 TRES=3 || env | grep TRES
    -   mini output = ()
    -   bash output = (TRES=3)


<a id="org62a4e50"></a>

# ./tester bonus

Test   2: ❌ cat <minishell.h&ls
mini output = ()
bash output = (bash<sub>outfiles</sub> bash.supp bonus bonus<sub>bonus</sub> builtins extras local.supp loop.out manual<sub>tests</sub> mini<sub>outfiles</sub> os<sub>specific</sub> outfile01 outfiles pipes README.md redirects syntax tester test<sub>files</sub> wildcards)
mini exit code = 1
bash exit code = 0
Test   5: ❌ &
mini exit code = 127
bash exit code = 2
mini error = ( &)
bash error = ( syntax error near unexpected token \`&&rsquo;)
Test   8: ❌ \*
mini exit code = 0
bash exit code = 127
mini error = ()

Test  19: ❌ << \*
mini exit code = 2
bash exit code = 0


<a id="orgc17f56d"></a>

# ./tester wildcards

Test   8: ❌ echo &ldquo;**"
mini output = (bash.supp bash<sub>outfiles</sub> bonus bonus<sub>bonus</sub> builtins extras local.supp loop.out manual<sub>tests</sub> mini<sub>outfiles</sub> os<sub>specific</sub> outfiles pipes README.md redirects syntax test<sub>files</sub> tester wildcards)
bash output = (**)
Test   9: ❌ &rdquo;**"
mini exit code = 0
bash exit code = 127
mini error = ()
bash error = ( command not found)
Test  10: ❌ manual**
mini exit code = 0
bash exit code = 127
mini error = ()
bash error = ( command not found)
Test  16: ❌ echo &ldquo;pip\*&rdquo;
mini output = (pipes)
bash output = (pip\*)

