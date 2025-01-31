
# Table of Contents

1.  [./tester](#org5b54bd2)
2.  [./tester redirects](#org6a8ceb4)
3.  [tester extras](#org32b5e9a)
4.  [tester os<sub>specific</sub>](#org6cb646a)
5.  [./tester bonus](#org0102ebd)
6.  [./tester wildcards](#orgf5ef85d)



<a id="org5b54bd2"></a>

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


<a id="org6a8ceb4"></a>

# ./tester redirects

-   Test  68: ✅⚠️  echo <&ldquo;./test<sub>files</sub>/infile<sub>big</sub>&rdquo; | echo <&ldquo;./test<sub>files</sub>/infile&rdquo;
-   mini error = ()
-   bash error = ( Broken pipe)
-   Test  71: ✅⚠️  cat <&ldquo;./test<sub>files</sub>/infile&rdquo; | echo hi
-   mini error = ()
-   bash error = ( Broken pipe)


<a id="org32b5e9a"></a>

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


<a id="org6cb646a"></a>

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


<a id="org0102ebd"></a>

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


<a id="orgf5ef85d"></a>

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

