#! /bin/sh
##
##				Gurich
##	**	Ricoh SP110 series driver **
##
##	Copyright (C) 2016, 2017 Gustaf Haglund <ghaglund@bahnhof.se>
##
##	This program is free software: you can redistribute it and/or modify
##	it under the terms of the GNU General Public License as published by
##	the Free Software Foundation, either version 3 of the License, or
##	(at your option) any later version.
##
##	This program is distributed in the hope that it will be useful,
##	but WITHOUT ANY WARRANTY; without even the implied warranty of
##	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##	GNU General Public License for more details.
##
##	You should have received a copy of the GNU General Public License
##	along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

curl https://raw.githubusercontent.com/madlynx/ricoh-sp100/master/RICOH_Aficio_SP_100.ppd -OSP112.ppd
echo "You will need to change the cupsFilter line to something like this: '*cupsFilter: \"application/vnd.cups-postscript 100 gurich\"'"
