use TF_KHJ
go

drop table Master_200405_3
go

Create table Master_200405_3
(IDN Char(6), 
 FS301 int, FS302 int, FS303 int, FS304 int, FS305 int, FS306 int, 
 FS307_3M int, FS307_6M int, FS307_9M int, FS307_12M int, FS308 int,
 FS309 int, FS310 int, FS311 int, FS312 int, FS313 int, FS314 int,
 MS301 int, MS302 int, MS303 int, MS304 int, MS305 int, MS306 int,
 MS307 float, MS308 float, MS309 float, MS310 float, MS311 float,
 MS312 float, MS313 float, MS314 float)
GO 

Insert into Master_200405_3
select IDN, 
       FS301, FS302, FS303, FS304, FS305, FS306,
       FS307_3M, FS307_6M, FS307_9M, FS307_12M,
       FS308, FS309, FS310, FS311, FS312, FS313, FS314,
       null, null, null, null, null, null, null,
       null, null, null, null, null, null, null
From FS3xx_200405
GO

update Master_200405_3
set 
MS301 = A.MS301,
MS302 = A.MS302,
MS303 = A.MS303,
MS304 = A.MS304,
MS305 = A.MS305,
MS306 = A.MS306,
MS307 = A.MS307,
MS308 = A.MS308,
MS309 = A.MS309,
MS310 = A.MS310,
MS311 = A.MS311,
MS312 = A.MS312,
MS313 = A.MS313,
MS314 = A.MS314
from MS3xx_200405 AS a
where a.IDN=Master_200405_3.IDN
GO


alter table Master_200405_3
add 
FT307_43 int, FT307_42 int, FT307_41 int, FT307_32 int, FT307_31 int, FT307_21 int	
GO

update Master_200405_3
set 
FT307_43 = 2 * FS307_3M - FS307_6M, FT307_42 = FS307_3M - FS307_9M + FS307_6M, FT307_41 = FS307_3M - FS307_12M + FS307_9M, FT307_31 = FS307_6M - FS307_3M - FS307_12M + FS307_9M, FT307_32 = 2 * FS307_6M - FS307_3M - FS307_9M, FT307_21 = 2 * FS307_9M - FS307_6M - FS307_12M
go

