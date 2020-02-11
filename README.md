mathml-tests<img width="14%" align="right" src=".graphics/sbml-badge.svg">
============

*Authors*:      [Lucian Smith](https://github.com/luciansmith).

*License*:      This code is licensed under the LGPL version 2.1.  Please see the file [LICENSE.txt](https://raw.githubusercontent.com/sbmlteam/mathml-tests/master/LICENSE.txt) for details.

*Repository*:   [https://github.com/sbmlteam/mathml-tests](https://github.com/sbmlteam/mathml-tests)

*Developers' discussion group*: [https://groups.google.com/forum/#!forum/libsbml-development](https://groups.google.com/forum/#!forum/libsbml-development)

-----

This repository contains tests for infix-to-mathML and mathML-to-infix for both [libSBML](http://sbml.org/Software/libSBML) and JSBML.

The files `summary.txt` and `summary.xlsx` are spreadsheets containing a summary of the tests.  The first column is the infix input used; the second is the MathML output; the third is the infix ouput (after being round-tripped), and the fourth column contains alternate round-trip infix output if [JSBML](http://sbml.org/Software/JSBML) is used instead of [libSBML](http://sbml.org/Software/libSBML).

The [dev](dev) directory contains a C++ program to create these tests, which works with [libSBML](http://sbml.org/Software/libSBML).

The [tests](tests) directory contains all the individual tests themselves.  Each test is an XML file containing MathML.  The infix input used to create that MathML is included as a MathML `<annotation>` element with the encoding 'infix-input'::

    <annotation encoding="infix-input">a</annotation>

If both [libSBML](http://sbml.org/Software/libSBML) and [JSBML](http://sbml.org/Software/JSBML) produce the same infix output from this MathML, this will be annotated with the encoding 'infix-output':

    <annotation encoding="infix-output">a</annotation>

If [libSBML](http://sbml.org/Software/libSBML) and [JSBML](http://sbml.org/Software/JSBML) produce different (but correct) infix output from this MathML, each output will be included as a separate annotation, the first with the encoding 'infix-output-libsbml', and the second with the encoding 'infix-output-jsbml':

    <annotation encoding="infix-output-libsbml">1.000000e+003</annotation>
    <annotation encoding="infix-output-jsbml">1E3</annotation>

Note that [libSBML](http://sbml.org/Software/libSBML) contains options in its infix parser to change how it parses certain input.  None of these options are tested here, and the defaults are used throughout.  In particular, this means:

* `log(x)` is always parsed as the natural log of x.
* Unary minuses (`-5`, `--5`, etc.) are not collapsed, and will always appear in the MathML with the `<minus>` element:

```
      <apply>
        <minus/>
        <cn type="integer"> 3 </cn>
      </apply>
```
instead of:
```
      <cn type="integer"> -3 </cn>
```

* Numerical values followed by a string are parsed as that value, with units of the string (`"5 mL"`).
* The string `"avogadro"` is parsed as the SMBL Level 3 _csymbol_ 'avogadro'.
* All parsing of built-in functions and constants is case-insensitive.


Copyright and license
---------------------

These files are distributed freely under the terms of the LGPL.  Please see the file [LICENSE.txt](LICENSE.txt) for more copyright and license details.


Acknowledgments
---------------

Funding for this and other SBML work has come from the [National Institute of General Medical Sciences](https://www.nigms.nih.gov) via grant NIH R01&nbsp;GM070923 (Principal Investigator: Michael Hucka).

<br>
<div align="center">
  <a href="https://www.nigms.nih.gov">
    <img valign="middle"  height="100" src=".graphics/US-NIH-NIGMS-Logo.svg">
  </a>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <a href="https://www.caltech.edu">
    <img valign="middle" height="130" src=".graphics/caltech-round.png">
  </a>
</div>
