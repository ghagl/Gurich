Gurich
-----

Gurich is a free printer driver for Ricoh SP112.
Other printers in the Ricoh SP100/Ricoh SP110 series probably work too.

Contributions to this project are welcome, but please follow the code style used in this driver.

In order to install this driver, please see the INSTALL file.

If you want a driver with warranty, then use Ricoh's driver for Microsoft Windows.

I am neither affilated with nor employed by Ricoh, this project is done in my spare time.
All trademarks mentioned in this project are owned by their respective owner.

## Features
* Printing (A4 with 600 and 1200 DPI are tested and work reliably)
* Status reports (good, bad, etc)
* Toner reports (how much toner is left)
* Can print the printer's built-in testpage

The support for CUPS is limited. There's a backend and filter you can use,
but the options for e.g. paper type (A4, A5, etc.) are currently not implemented.

## Usage
You will store usually postscript files in printfiles/ and then letting the driver do the job,
otherwise you can pair together the PPD file in ppd/ with CUPS. See the INSTALL file.

PDFs work, too.

An example for printing:

    $ ./bin/gurich -p printfiles/gplv3.ps 600 1 A4

For other use cases, e.g seeing how much toner is left:

    $ ./bin/gurich -s
    [...]
    Printer status: GOOD / ENERGY SAVING MODE 2 / IDLE, reference: 0x31
    Printer toner: 80 % left
    Printed out pages (stats): 301

    $ ./bin/gurich -h
    [...]
    print: ./bin/gurich -p [ps/pdf file, resolution (600|1200), copies, paper type (A4 is tested)]
    status: ./bin/gurich -s
    testpage: ./bin/gurich -t

You might need to make a rule for udev in order to avoid running gurich with root privileges,
on GNU/Linux-based systems.
