USE TF_KHJ
GO

drop table y_BAM085_200410
go
create table y_BAM085_200410 (IDN Char(6), max_bucket float)
go

insert into y_BAM085_200410
select IDN, max(bucket)
from y_BAM085_tmp_200410
WHERE ACCOUNT_CODE = 'Y'
group by IDN
go

select * from bam085_200410 where account_code = 'Y'
