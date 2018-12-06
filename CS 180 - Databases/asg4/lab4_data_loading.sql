-- Observe "\N" means NULL.

COPY Lab4.Authors FROM stdin USING DELIMITERS '|' ;
643|Elizabeth Conrad|9 Sierra St Roslindale, MA|28|1682-11-14
2789|Towanda Chastain|25 Oxford St Oshkosh, WI|14|1682-11-21
2222|Aurelio Beller|\N|56|1691-10-05
3333|Hank Souders|72 Sierra St Irwin, PA|62|1729-06-30
5555|Hank Souders|72 Nevada St Steve, MA|62|1729-06-30
2192|Darin Blanca|120 Bohemia St Bloomfield, NJ|26|1810-06-04
5068|Carlotta Carrington|8787 Temple St Vicksburg, MS|29|1850-10-11
2439|Zachery Tocci|373 Rockwell Rd Chicago, IL|71|1865-05-28
3719|Denny Schepis|7505 Pablo Ave Good, TN|9|1924-03-15
1561|Monica Collar|93 West Kent St Lansing, MI|3|1933-08-11
4017|Pura Whitener|97 Hall Ln Prattville, AL|59|1939-07-04
9164|Odell Kiely|8595 Jefferson Dr Lansdale, PA|99|1958-09-03
9165|Sara McDeally|2211 Tunst St Lansdale, PA|99|1958-09-03
1111|Charlotte Bronte|\N|7|1846-01-01
4444|John Smith|\N|8|\N
1306|Jane Doe|\N|8|\N
111|Eric McDeally|F St Lansdale, PA|99|1958-09-03
222|Shauna McDeally|G St Lansdale, PA|99|1958-09-03
3372|Allen McDeally|I St Lansdale, PA|99|1958-09-03
\.

COPY Lab4.Books FROM stdin USING DELIMITERS '|' ;
ccfyls|643|Foe Without A Home|92152|1682-11-14|9300.65|F|2018-01-11|19
kgoetm|2789|Doctor Of The Plague|17265|\N|400.55|F|2018-03-14|16
zdulmo|1306|Armies Of The Great|55576|1729-06-30|52.45|F|2018-06-10|11
ydbymx|2192|Dogs And Cats|94519|1810-06-04|89.4|F|2018-07-01|22
tyuiop|2192|Cats And Dogs|94519|1810-06-04|89.4|F|2018-07-01|22
sdfghj|2192|Cats|94519|1810-06-04|99.4|F|2018-07-01|22
dfghjk|2192|Dogs|94519|1810-06-04|90.50|F|2018-07-01|22
mfasyc|5068|Body Of The Lost Ones|32326|1850-10-11|20.64|F|2018-08-06|5
cekpqu|2439|Edge Of My Imagination|93209|1865-05-28|23.93|F|2018-09-11|13
mibxyk|3719|Sailing Into The Champions|22778|1924-03-15|650.26|F|2018-09-18|25
dmfnth|1561|Bow Of Freedom|98035|1933-08-11|8.72|F|2018-10-04|12
jgzhwq|111|Jumping Into The River|31725|2013-07-28|24.09|F|2018-11-12|19
qwerty|111|Leaping Into The Ocean|31725|2013-07-28|24.09|F|2018-11-12|19
paudhc|222|Battling At The Depths|22139|2017-09-21|9.46|F|2018-12-01|27
paudh1|222|Battling At The Roots|22139|2017-09-21|9.46|N|2018-12-01|27
paudh2|222|Battling At The Shallows|22139|2017-09-21|9.46|F|2018-12-01|27
asdfqw|111|Falling Into The Ocean|31725|2013-08-28|23.99|F|2018-11-12|13
zxcvas|2192|Animalia Unbound|94519|1990-07-04|9.99|F|2000-07-10|67
rrrrrr|1111|Jane Eyre|94519|2000-01-31|29.99|F|2018-07-10|100
aaaaaa|2222|Thatz Random|94519|\N|39.99|N|2018-07-10|100
bbbbbb|3333|Thatz Random|94519|2011-01-31|29.99|N|2020-07-10|100
ffffff|5555|Thatz Random|94519|2000-01-31|29.99|N|2018-07-10|100
cccccc|4444|The History of Virginia|98035|2011-04-01|9.99|F|2012-04-01|10
eeeeee|4444|A Map of Virginia|98035|2011-04-01|9.99|F|2012-04-01|10
dddddd|9165|Jane Eyre|98035|2011-04-01|29.99|F|2012-04-01|10
jvlfno|3372|Thieves Of Freedom|25327|1691-10-05|63.5|F|2018-04-21|6
\.

COPY Lab4.Members FROM stdin USING DELIMITERS '|' ;
4843|Debbi Geissler|2001-07-23|2019-07-23|TRUE
0000|Abbie Knight|2001-07-23|2019-07-23|TRUE
8000|Khadijah Beeson|2004-07-29|2019-07-29|TRUE
9315|Wendy Sacks|2005-12-09|2019-12-09|TRUE
9557|Janell Bagnall|2006-06-25|2019-06-25|FALSE
7447|Emil Cover|2011-12-08|2019-12-08|TRUE
1794|Annis Hylton|2014-04-08|2019-04-08|TRUE
9755|Amy Astorga|2014-07-13|2019-07-13|FALSE
2161|Kathi Renfroe|2015-06-12|2019-06-12|FALSE
8737|Remedios Grim|2017-11-10|2019-11-10|FALSE
8844|Carola Quinley|2017-12-10|2019-12-10|TRUE
6247|Edison Alto|2018-06-05|2019-06-05|TRUE
2222|John Smith|2018-06-05|2019-06-05|FALSE
3333|John Smith|2018-07-05|2019-07-05|TRUE
4444|John Smith|2018-08-05|2019-08-05|TRUE
57995|Jenny Who|2018-08-05|2019-08-05|TRUE
66269|Alex Smith|2018-08-05|2019-08-05|TRUE
87252|Cory Smith|2018-08-05|2019-08-05|TRUE
84681|Erin Smith|2018-08-05|2019-08-05|TRUE
88596|Frances Smith|2018-08-05|2019-08-05|TRUE
96542|Gayle Smith|2018-08-05|2019-08-05|TRUE
79320|Io Smith|2018-08-05|2019-08-05|TRUE
30108|Kelsey Smith|2018-08-05|2019-08-05|TRUE
85398|Leslie Smith|2018-08-05|2019-08-05|TRUE
75241|Mackenzie Smith|2018-08-05|2019-08-05|TRUE
27550|Nevada Smith|2018-08-05|2019-08-05|TRUE
\.

COPY Lab4.Orders FROM stdin USING DELIMITERS '|' ;
4843|ccfyls|2006-03-04|6
4444|ccfyls|2006-03-04|6
8000|kgoetm|2006-08-05|10
9315|jvlfno|2007-10-18|2
9557|zdulmo|2008-01-12|2
7447|ydbymx|2008-03-15|11
1794|mfasyc|2008-04-10|4
9755|cekpqu|2009-01-30|1
2161|mibxyk|2009-09-14|8
8737|dmfnth|2010-03-08|9
6247|paudhc|2012-01-15|4
2161|ccfyls|2012-09-15|7
8737|kgoetm|2012-09-21|6
8844|jvlfno|2012-12-19|4
6247|zdulmo|2013-05-23|9
9557|ydbymx|2013-09-03|10
7447|mfasyc|2014-04-11|6
1794|cekpqu|2014-06-07|2
9755|mibxyk|2014-10-15|9
2161|dmfnth|2014-12-13|3
8844|paudhc|2017-10-03|1
2161|mibxyk|2016-01-31|8
8844|jgzhwq|2011-05-19|9
2161|rrrrrr|2016-01-31|99
\.

COPY Lab4.Publishers FROM stdin USING DELIMITERS '|' ;
92152|Laudise House|38 Sunny St Bowl, VA
17265|Binyx Inc.|7100 Stony Ave Jamaica, NY
25327|Katude Publishers|88 Holly St Boynton Beach, FL
55576|Antemm House|68 Golf St Greenfield, IN
94519|Difity Ltd.|25 Mountain Dr Mableton, GA
32326|Superou Corp.|8270 Wood St Rochester, NY
93209|Archity Bros.|203 Morris Ave Beltsville, MD
22778|Zanyx Publishing Group  |5 Piper Crt  Erie, PA
98035|Omizu House|164 Pablo Ave Alexandria, VA
31725|Symity Publishing|369 View St. Highland, IL
22139|Semiveo House|26 Creek St Frederick, VA
22140|Biveo House|26 Stream St Frederick, VA
\.

COPY Lab4.Reviews FROM stdin USING DELIMITERS '|' ;
57995|ccfyls|2006-05-26|5
66269|ccfyls|2006-06-30|5
57995|paudhc|2018-01-05|3
87252|ydbymx|2007-08-10|1
87252|tyuiop|2007-08-10|1
87252|zxcvas|2007-08-10|1
27550|ydbymx|2007-11-15|3
27550|dfghjk|2007-11-15|3
27550|sdfghj|2007-11-15|3
27550|zxcvas|2007-11-15|3
84681|cekpqu|2008-08-31|4
88596|cekpqu|2009-02-04|3
96542|qwerty|2017-08-23|4
96542|jgzhwq|2017-08-23|3
85398|jgzhwq|2011-04-15|4
79320|paudhc|2011-06-10|2
88596|ccfyls|2011-06-16|5
30108|ccfyls|2013-09-07|4
85398|ccfyls|2014-01-22|5
75241|ydbymx|2015-03-04|5
75241|zxcvas|2015-03-04|5
27550|cekpqu|2016-07-25|5
2222|zxcvas|2016-07-25|5
2222|rrrrrr|2016-07-25|5
2222|cccccc|2016-07-25|1
2222|dddddd|2016-07-25|4
3333|cccccc|2016-07-25|3
3333|eeeeee|2016-07-25|3
3333|dddddd|2016-07-25|1
3333|rrrrrr|2016-07-25|1
4444|dddddd|2016-07-25|5
4444|asdfqw|2016-07-25|5
96542|ccfyls|2006-05-23|2
\.

COPY Lab4.NewMemberships FROM stdin USING DELIMITERS '|' ;
1234|Alex Zedd|2020-07-23
5678|Bailey Yan|2020-07-23
9101|Cory Xavier|2020-07-23
1112|Devyn Webb|2020-07-23
1213|Erin Vane|2020-07-23
9557|Jan Begnall|2020-07-23
2222|Jack Smith|2020-01-01
8000|Khadijah Beson|2021-12-15
4843|Debbi Winters|2022-11-14
30108|Kelsey Grammar Smith|2023-10-13
\.
