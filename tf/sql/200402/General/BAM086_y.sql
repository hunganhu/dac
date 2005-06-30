USE TF_KHJ
GO

drop table y_BAM085_200402
go
create table y_BAM085_200402 (IDN Char(6), max_bucket float)
go

insert into y_BAM085_200402
select IDN, max(bucket)
from y_BAM085_tmp_200402
WHERE ACCOUNT_CODE = 'Y'
group by IDN
go

select * from bam085_200402 where account_code = 'Y'
