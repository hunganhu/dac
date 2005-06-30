USE TF_GX
GO

INSERT INTO MASTER_BAM085_1
(IDN,
 RS101, RS102, RS103, RS104, 
 RS105, RS106, RS107, RS108, 
 RS109, RS110, RS111, RS112, 
 RS113, RS114, RS115, RS116, 
 RS117, RS118, RS119, RS120)
SELECT IDN,
 RS101, RS102, RS103, RS104, 
 RS105, RS106, RS107, RS108, 
 RS109, RS110, RS111, RS112, 
 RS113, RS114, RS115, RS116, 
 RS117, RS118, RS119, RS120
 FROM BAM085_RS_200304
GO

UPDATE MASTER_BAM085_1
SET
 MS315_3M=A. MS315_3M,	 MS315_6M=A. MS315_6M,	 MS315_9M=A. MS315_9M,	 MS315_12M=A. MS315_12M,
 MS316_3M=A. MS316_3M,	 MS316_6M=A. MS316_6M,	 MS316_9M=A. MS316_9M,	 MS316_12M=A. MS316_12M,
 MS317_3M=A. MS317_3M,	 MS317_6M=A. MS317_6M,	 MS317_9M=A. MS317_9M,	 MS317_12M=A. MS317_12M,
 MS318_3M=A. MS318_3M,	 MS318_6M=A. MS318_6M,	 MS318_9M=A. MS318_9M,	 MS318_12M=A. MS318_12M,
 MS319_3M=A. MS319_3M,	 MS319_6M=A. MS319_6M,	 MS319_9M=A. MS319_9M,	 MS319_12M=A. MS319_12M,
 MS320_3M=A. MS320_3M,	 MS320_6M=A. MS320_6M,	 MS320_9M=A. MS320_9M,	 MS320_12M=A. MS320_12M,
 MS321_3M=A. MS321_3M,	 MS321_6M=A. MS321_6M,	 MS321_9M=A. MS321_9M,	 MS321_12M=A. MS321_12M,
 MS322_3M=A. MS322_3M,	 MS322_6M=A. MS322_6M,	 MS322_9M=A. MS322_9M,	 MS322_12M=A. MS322_12M,
 MS323_3M=A. MS323_3M,	 MS323_6M=A. MS323_6M,	 MS323_9M=A. MS323_9M,	 MS323_12M=A. MS323_12M,
 MS324_3M=A. MS324_3M,	 MS324_6M=A. MS324_6M,	 MS324_9M=A. MS324_9M,	 MS324_12M=A. MS324_12M,
 MS325_3M=A. MS325_3M,	 MS325_6M=A. MS325_6M,	 MS325_9M=A. MS325_9M,	 MS325_12M=A. MS325_12M,
 MS326_3M=A. MS326_3M,	 MS326_6M=A. MS326_6M,	 MS326_9M=A. MS326_9M,	 MS326_12M=A. MS326_12M,
 MS327_3M=A. MS327_3M,	 MS327_6M=A. MS327_6M,	 MS327_9M=A. MS327_9M,	 MS327_12M=A. MS327_12M,
 MS328_3M=A. MS328_3M,	 MS328_6M=A. MS328_6M,	 MS328_9M=A. MS328_9M,	 MS328_12M=A. MS328_12M,
 MS329_3M=A. MS329_3M,	 MS329_6M=A. MS329_6M,	 MS329_9M=A. MS329_9M,	 MS329_12M=A. MS329_12M,
 MS330_3M=A. MS330_3M,	 MS330_6M=A. MS330_6M,	 MS330_9M=A. MS330_9M,	 MS330_12M=A. MS330_12M,
 MS331_3M=A. MS331_3M,	 MS331_6M=A. MS331_6M,	 MS331_9M=A. MS331_9M,	 MS331_12M=A. MS331_12M,
 MS332_3M=A. MS332_3M,	 MS332_6M=A. MS332_6M,	 MS332_9M=A. MS332_9M,	 MS332_12M=A. MS332_12M,
 MS333_3M=A. MS333_3M,	 MS333_6M=A. MS333_6M,	 MS333_9M=A. MS333_9M,	 MS333_12M=A. MS333_12M,
 MS334_3M=A. MS334_3M,	 MS334_6M=A. MS334_6M,	 MS334_9M=A. MS334_9M,	 MS334_12M=A. MS334_12M,
 MS335_3M=A. MS335_3M,	 MS335_6M=A. MS335_6M,	 MS335_9M=A. MS335_9M,	 MS335_12M=A. MS335_12M,
 MS336_3M=A. MS336_3M,	 MS336_6M=A. MS336_6M,	 MS336_9M=A. MS336_9M,	 MS336_12M=A. MS336_12M,
 MS337_3M=A. MS337_3M,	 MS337_6M=A. MS337_6M,	 MS337_9M=A. MS337_9M,	 MS337_12M=A. MS337_12M,
 MS338_3M=A. MS338_3M,	 MS338_6M=A. MS338_6M,	 MS338_9M=A. MS338_9M,	 MS338_12M=A. MS338_12M,
 MS339_3M=A. MS339_3M,	 MS339_6M=A. MS339_6M,	 MS339_9M=A. MS339_9M,	 MS339_12M=A. MS339_12M,
 MS340_3M=A. MS340_3M,	 MS340_6M=A. MS340_6M,	 MS340_9M=A. MS340_9M,	 MS340_12M=A. MS340_12M,
 MS341_3M=A. MS341_3M,	 MS341_6M=A. MS341_6M,	 MS341_9M=A. MS341_9M,	 MS341_12M=A. MS341_12M,
 MS342_3M=A. MS342_3M,	 MS342_6M=A. MS342_6M,	 MS342_9M=A. MS342_9M,	 MS342_12M=A. MS342_12M,
 MS343_3M=A. MS343_3M,	 MS343_6M=A. MS343_6M,	 MS343_9M=A. MS343_9M,	 MS343_12M=A. MS343_12M,
 MS344_3M=A. MS344_3M,	 MS344_6M=A. MS344_6M,	 MS344_9M=A. MS344_9M,	 MS344_12M=A. MS344_12M,
 MS345_3M=A. MS345_3M,	 MS345_6M=A. MS345_6M,	 MS345_9M=A. MS345_9M,	 MS345_12M=A. MS345_12M,
 MS346_3M=A. MS346_3M,	 MS346_6M=A. MS346_6M,	 MS346_9M=A. MS346_9M,	 MS346_12M=A. MS346_12M,
 MS347_3M=A. MS347_3M,	 MS347_6M=A. MS347_6M,	 MS347_9M=A. MS347_9M,	 MS347_12M=A. MS347_12M,
 MS348_3M=A. MS348_3M,	 MS348_6M=A. MS348_6M,	 MS348_9M=A. MS348_9M,	 MS348_12M=A. MS348_12M,
 MS349_3M=A. MS349_3M,	 MS349_6M=A. MS349_6M,	 MS349_9M=A. MS349_9M,	 MS349_12M=A. MS349_12M,
 MS350_3M=A. MS350_3M,	 MS350_6M=A. MS350_6M,	 MS350_9M=A. MS350_9M,	 MS350_12M=A. MS350_12M,
 MS351_3M=A. MS351_3M,	 MS351_6M=A. MS351_6M,	 MS351_9M=A. MS351_9M,	 MS351_12M=A. MS351_12M,
 MS352_3M=A. MS352_3M,	 MS352_6M=A. MS352_6M,	 MS352_9M=A. MS352_9M,	 MS352_12M=A. MS352_12M,
 MS353_3M=A. MS353_3M,	 MS353_6M=A. MS353_6M,	 MS353_9M=A. MS353_9M,	 MS353_12M=A. MS353_12M,
 MS354_3M=A. MS354_3M,	 MS354_6M=A. MS354_6M,	 MS354_9M=A. MS354_9M,	 MS354_12M=A. MS354_12M,
 MS355_3M=A. MS355_3M,	 MS355_6M=A. MS355_6M,	 MS355_9M=A. MS355_9M,	 MS355_12M=A. MS355_12M,
 MS356_3M=A. MS356_3M,	 MS356_6M=A. MS356_6M,	 MS356_9M=A. MS356_9M,	 MS356_12M=A. MS356_12M,
 MS357_3M=A. MS357_3M,	 MS357_6M=A. MS357_6M,	 MS357_9M=A. MS357_9M,	 MS357_12M=A. MS357_12M,
 MS358_3M=A. MS358_3M,	 MS358_6M=A. MS358_6M,	 MS358_9M=A. MS358_9M,	 MS358_12M=A. MS358_12M,
 MS359_3M=A. MS359_3M,	 MS359_6M=A. MS359_6M,	 MS359_9M=A. MS359_9M,	 MS359_12M=A. MS359_12M,
 MS360_3M=A. MS360_3M,	 MS360_6M=A. MS360_6M,	 MS360_9M=A. MS360_9M,	 MS360_12M=A. MS360_12M,
 MS361_3M=A. MS361_3M,	 MS361_6M=A. MS361_6M,	 MS361_9M=A. MS361_9M,	 MS361_12M=A. MS361_12M,
 MS363_3M=A. MS363_3M,	 MS363_6M=A. MS363_6M,	 MS363_9M=A. MS363_9M,	 MS363_12M=A. MS363_12M,
 MS364_3M=A. MS364_3M,	 MS364_6M=A. MS364_6M,	 MS364_9M=A. MS364_9M,	 MS364_12M=A. MS364_12M,
 MS365_3M=A. MS365_3M,	 MS365_6M=A. MS365_6M,	 MS365_9M=A. MS365_9M,	 MS365_12M=A. MS365_12M,
 MS366_3M=A. MS366_3M,	 MS366_6M=A. MS366_6M,	 MS366_9M=A. MS366_9M,	 MS366_12M=A. MS366_12M,
 MS367_3M=A. MS367_3M,	 MS367_6M=A. MS367_6M,	 MS367_9M=A. MS367_9M,	 MS367_12M=A. MS367_12M,
 MS368_3M=A. MS368_3M,	 MS368_6M=A. MS368_6M,	 MS368_9M=A. MS368_9M,	 MS368_12M=A. MS368_12M,
 MS369_3M=A. MS369_3M,	 MS369_6M=A. MS369_6M,	 MS369_9M=A. MS369_9M,	 MS369_12M=A. MS369_12M,
 MS370_3M=A. MS370_3M,	 MS370_6M=A. MS370_6M,	 MS370_9M=A. MS370_9M,	 MS370_12M=A. MS370_12M,
 MS371_3M=A. MS371_3M,	 MS371_6M=A. MS371_6M,	 MS371_9M=A. MS371_9M,	 MS371_12M=A. MS371_12M,
 MS372_3M=A. MS372_3M,	 MS372_6M=A. MS372_6M,	 MS372_9M=A. MS372_9M,	 MS372_12M=A. MS372_12M,
 MS373_3M=A. MS373_3M,	 MS373_6M=A. MS373_6M,	 MS373_9M=A. MS373_9M,	 MS373_12M=A. MS373_12M,
 MS374_3M=A. MS374_3M,	 MS374_6M=A. MS374_6M,	 MS374_9M=A. MS374_9M,	 MS374_12M=A. MS374_12M,
 MS375_3M=A. MS375_3M,	 MS375_6M=A. MS375_6M,	 MS375_9M=A. MS375_9M,	 MS375_12M=A. MS375_12M,
 MS376_3M=A. MS376_3M,	 MS376_6M=A. MS376_6M,	 MS376_9M=A. MS376_9M,	 MS376_12M=A. MS376_12M,
 MS377_3M=A. MS377_3M,	 MS377_6M=A. MS377_6M,	 MS377_9M=A. MS377_9M,	 MS377_12M=A. MS377_12M,
 MS378_3M=A. MS378_3M,	 MS378_6M=A. MS378_6M,	 MS378_9M=A. MS378_9M,	 MS378_12M=A. MS378_12M,
 MS379_3M=A. MS379_3M,	 MS379_6M=A. MS379_6M,	 MS379_9M=A. MS379_9M,	 MS379_12M=A. MS379_12M,
 MS380_3M=A. MS380_3M,	 MS380_6M=A. MS380_6M,	 MS380_9M=A. MS380_9M,	 MS380_12M=A. MS380_12M,
 MS381_3M=A. MS381_3M,	 MS381_6M=A. MS381_6M,	 MS381_9M=A. MS381_9M,	 MS381_12M=A. MS381_12M,
 MS382_3M=A. MS382_3M,	 MS382_6M=A. MS382_6M,	 MS382_9M=A. MS382_9M,	 MS382_12M=A. MS382_12M,
 MS383_3M=A. MS383_3M,	 MS383_6M=A. MS383_6M,	 MS383_9M=A. MS383_9M,	 MS383_12M=A. MS383_12M,
 MS385_3M=A. MS385_3M,	 MS385_6M=A. MS385_6M,	 MS385_9M=A. MS385_9M,	 MS385_12M=A. MS385_12M,
 MS386_3M=A. MS386_3M,	 MS386_6M=A. MS386_6M,	 MS386_9M=A. MS386_9M,	 MS386_12M=A. MS386_12M,
 MS387_3M=A. MS387_3M,	 MS387_6M=A. MS387_6M,	 MS387_9M=A. MS387_9M,	 MS387_12M=A. MS387_12M,
 MS388_3M=A. MS388_3M,	 MS388_6M=A. MS388_6M,	 MS388_9M=A. MS388_9M,	 MS388_12M=A. MS388_12M,
 MS389_3M=A. MS389_3M,	 MS389_6M=A. MS389_6M,	 MS389_9M=A. MS389_9M,	 MS389_12M=A. MS389_12M,
 MS390_3M=A. MS390_3M,	 MS390_6M=A. MS390_6M,	 MS390_9M=A. MS390_9M,	 MS390_12M=A. MS390_12M,
 MS391_3M=A. MS391_3M,	 MS391_6M=A. MS391_6M,	 MS391_9M=A. MS391_9M,	 MS391_12M=A. MS391_12M,
 MS392_3M=A. MS392_3M,	 MS392_6M=A. MS392_6M,	 MS392_9M=A. MS392_9M,	 MS392_12M=A. MS392_12M,
 MS393_3M=A. MS393_3M,	 MS393_6M=A. MS393_6M,	 MS393_9M=A. MS393_9M,	 MS393_12M=A. MS393_12M,
 MS394_3M=A. MS394_3M,	 MS394_6M=A. MS394_6M,	 MS394_9M=A. MS394_9M,	 MS394_12M=A. MS394_12M,
 MS395_3M=A. MS395_3M,	 MS395_6M=A. MS395_6M,	 MS395_9M=A. MS395_9M,	 MS395_12M=A. MS395_12M,
 MS396_3M=A. MS396_3M,	 MS396_6M=A. MS396_6M,	 MS396_9M=A. MS396_9M,	 MS396_12M=A. MS396_12M,
 MS397_3M=A. MS397_3M,	 MS397_6M=A. MS397_6M,	 MS397_9M=A. MS397_9M,	 MS397_12M=A. MS397_12M,
 MS398_3M=A. MS398_3M,	 MS398_6M=A. MS398_6M,	 MS398_9M=A. MS398_9M,	 MS398_12M=A. MS398_12M,
 MS399_3M=A. MS399_3M,	 MS399_6M=A. MS399_6M,	 MS399_9M=A. MS399_9M,	 MS399_12M=A. MS399_12M,
 MS400_3M=A. MS400_3M,	 MS400_6M=A. MS400_6M,	 MS400_9M=A. MS400_9M,	 MS400_12M=A. MS400_12M,
 MS401_3M=A. MS401_3M,	 MS401_6M=A. MS401_6M,	 MS401_9M=A. MS401_9M,	 MS401_12M=A. MS401_12M,
 MS402_3M=A. MS402_3M,	 MS402_6M=A. MS402_6M,	 MS402_9M=A. MS402_9M,	 MS402_12M=A. MS402_12M,
 MS403_3M=A. MS403_3M,	 MS403_6M=A. MS403_6M,	 MS403_9M=A. MS403_9M,	 MS403_12M=A. MS403_12M,
 MS404_3M=A. MS404_3M,	 MS404_6M=A. MS404_6M,	 MS404_9M=A. MS404_9M,	 MS404_12M=A. MS404_12M
 FROM BAM085_MS_200304 AS A INNER JOIN MASTER_BAM085_1 AS B
ON A.IDN = B.IDN 
GO

INSERT INTO MASTER_BAM085_2
(IDN,
 FS334, FS315, FS316, FS317, 
 FS318, FS319, FS320, FS321, 
 FS322, FS323, FS324, FS325, 
 FS326, FS327, FS328, FS329, 
 FS330, FS331, FS332, FS333, 
 FS354_3M, FS354_6M, FS354_9M, FS354_12M, 
 FS355_3M, FS355_6M, FS355_9M, FS355_12M, 
 FS356_3M, FS356_6M, FS356_9M, FS356_12M, 
 FS357_3M, FS357_6M, FS357_9M, FS357_12M, 
 FS358_3M, FS358_6M, FS358_9M, FS358_12M, 
 FS359_3M, FS359_6M, FS359_9M, FS359_12M, 
 FS360_3M, FS360_6M, FS360_9M, FS360_12M, 
 FS361_3M, FS361_6M, FS361_9M, FS361_12M, 
 FS362_3M, FS362_6M, FS362_9M, FS362_12M, 
 FS363_3M, FS363_6M, FS363_9M, FS363_12M, 
 FS364_3M, FS364_6M, FS364_9M, FS364_12M, 
 FS365_3M, FS365_6M, FS365_9M, FS365_12M, 
 FS366_3M, FS366_6M, FS366_9M, FS366_12M, 
 FS367_3M, FS367_6M, FS367_9M, FS367_12M, 
 FS368_3M, FS368_6M, FS368_9M, FS368_12M, 
 FS369_3M, FS369_6M, FS369_9M, FS369_12M, 
 FS370_3M, FS370_6M, FS370_9M, FS370_12M, 
 FS371_3M, FS371_6M, FS371_9M, FS371_12M, 
 FS372_3M, FS372_6M, FS372_9M, FS372_12M, 
 FS373_3M, FS373_6M, FS373_9M, FS373_12M, 
 FS374_3M, FS374_6M, FS374_9M, FS374_12M, 
 FS375_3M, FS375_6M, FS375_9M, FS375_12M, 
 FS376_3M, FS376_6M, FS376_9M, FS376_12M, 
 FS377_3M, FS377_6M, FS377_9M, FS377_12M, 
 FS378_3M, FS378_6M, FS378_9M, FS378_12M, 
 FS379_3M, FS379_6M, FS379_9M, FS379_12M, 
 FS380_3M, FS380_6M, FS380_9M, FS380_12M, 
 FS381_3M, FS381_6M, FS381_9M, FS381_12M, 
 FS382_3M, FS382_6M, FS382_9M, FS382_12M, 
 FS383_3M, FS383_6M, FS383_9M, FS383_12M, 
 FS384_3M, FS384_6M, FS384_9M, FS384_12M, 
 FS385_3M, FS385_6M, FS385_9M, FS385_12M, 
 FS386_3M, FS386_6M, FS386_9M, FS386_12M, 
 FS387_3M, FS387_6M, FS387_9M, FS387_12M, 
 FS388_3M, FS388_6M, FS388_9M, FS388_12M, 
 FS389_3M, FS389_6M, FS389_9M, FS389_12M, 
 FS390_3M, FS390_6M, FS390_9M, FS390_12M, 
 FS391_3M, FS391_6M, FS391_9M, FS391_12M, 
 FS392_3M, FS392_6M, FS392_9M, FS392_12M,
 FS393_3M, FS393_6M, FS393_9M, FS393_12M,
 FS394_3M, FS394_6M, FS394_9M, FS394_12M,
 FS395_3M, FS395_6M, FS395_9M, FS395_12M,
 FS396_3M, FS396_6M, FS396_9M, FS396_12M,
 FS397_3M, FS397_6M, FS397_9M, FS397_12M,
 FS398_3M, FS398_6M, FS398_9M, FS398_12M,
 FS399_3M, FS399_6M, FS399_9M, FS399_12M,
 FS400_3M, FS400_6M, FS400_9M, FS400_12M,
 FS401_3M, FS401_6M, FS401_9M, FS401_12M,
 FS402_3M, FS402_6M, FS402_9M, FS402_12M,
 FS403_3M, FS403_6M, FS403_9M, FS403_12M,
 FS404_3M, FS404_6M, FS404_9M, FS404_12M,
 FS405_3M, FS405_6M, FS405_9M, FS405_12M,
 FS406_3M, FS406_6M, FS406_9M, FS406_12M,
 FS407_3M, FS407_6M, FS407_9M, FS407_12M,
 FS408_3M, FS408_6M, FS408_9M, FS408_12M,
 FS409_3M, FS409_6M, FS409_9M, FS409_12M,
 FS410_3M, FS410_6M, FS410_9M, FS410_12M,
 FS411_3M, FS411_6M, FS411_9M, FS411_12M,
 FS412_3M, FS412_6M, FS412_9M, FS412_12M,
 FS413_3M, FS413_6M, FS413_9M, FS413_12M,
 FS414_3M, FS414_6M, FS414_9M, FS414_12M,
 FS415_3M, FS415_6M, FS415_9M, FS415_12M,
 FS416_3M, FS416_6M, FS416_9M, FS416_12M,
 FS417_3M, FS417_6M, FS417_9M, FS417_12M,
 FS418_3M, FS418_6M, FS418_9M, FS418_12M,
 FS419_3M, FS419_6M, FS419_9M, FS419_12M,
 FS420_3M, FS420_6M, FS420_9M, FS420_12M,
 FS421_3M, FS421_6M, FS421_9M, FS421_12M,
 FS422_3M, FS422_6M, FS422_9M, FS422_12M,
 FS423_3M, FS423_6M, FS423_9M, FS423_12M,
 FS424_3M, FS424_6M, FS424_9M, FS424_12M,
 FS425_3M, FS425_6M, FS425_9M, FS425_12M,
 FS426_3M, FS426_6M, FS426_9M, FS426_12M,
 FS427_3M, FS427_6M, FS427_9M, FS427_12M,
 FS428_3M, FS428_6M, FS428_9M, FS428_12M,
 FS429_3M, FS429_6M, FS429_9M, FS429_12M,
 FS430_3M, FS430_6M, FS430_9M, FS430_12M,
 FS431_3M, FS431_6M, FS431_9M, FS431_12M,
 FS432_3M, FS432_6M, FS432_9M, FS432_12M,
 FS433_3M, FS433_6M, FS433_9M, FS433_12M,
 FS434_3M, FS434_6M, FS434_9M, FS434_12M,
 FS435_3M, FS435_6M, FS435_9M, FS435_12M,
 FS436_3M, FS436_6M, FS436_9M, FS436_12M,
 FS437_3M, FS437_6M, FS437_9M, FS437_12M,
 FS438_3M, FS438_6M, FS438_9M, FS438_12M,
 FS439_3M, FS439_6M, FS439_9M, FS439_12M,
 FS440_3M, FS440_6M, FS440_9M, FS440_12M,
 FS441_3M, FS441_6M, FS441_9M, FS441_12M,
 FS442_3M, FS442_6M, FS442_9M, FS442_12M,
 FS443_3M, FS443_6M, FS443_9M, FS443_12M,
 FS444_3M, FS444_6M, FS444_9M, FS444_12M,
 FS445_3M, FS445_6M, FS445_9M, FS445_12M)
SELECT
 IDN,
 FS334, FS315, FS316, FS317, 
 FS318, FS319, FS320, FS321, 
 FS322, FS323, FS324, FS325, 
 FS326, FS327, FS328, FS329, 
 FS330, FS331, FS332, FS333, 
 FS354_3M, FS354_6M, FS354_9M, FS354_12M, 
 FS355_3M, FS355_6M, FS355_9M, FS355_12M, 
 FS356_3M, FS356_6M, FS356_9M, FS356_12M, 
 FS357_3M, FS357_6M, FS357_9M, FS357_12M, 
 FS358_3M, FS358_6M, FS358_9M, FS358_12M, 
 FS359_3M, FS359_6M, FS359_9M, FS359_12M, 
 FS360_3M, FS360_6M, FS360_9M, FS360_12M, 
 FS361_3M, FS361_6M, FS361_9M, FS361_12M, 
 FS362_3M, FS362_6M, FS362_9M, FS362_12M, 
 FS363_3M, FS363_6M, FS363_9M, FS363_12M, 
 FS364_3M, FS364_6M, FS364_9M, FS364_12M, 
 FS365_3M, FS365_6M, FS365_9M, FS365_12M, 
 FS366_3M, FS366_6M, FS366_9M, FS366_12M, 
 FS367_3M, FS367_6M, FS367_9M, FS367_12M, 
 FS368_3M, FS368_6M, FS368_9M, FS368_12M, 
 FS369_3M, FS369_6M, FS369_9M, FS369_12M, 
 FS370_3M, FS370_6M, FS370_9M, FS370_12M, 
 FS371_3M, FS371_6M, FS371_9M, FS371_12M, 
 FS372_3M, FS372_6M, FS372_9M, FS372_12M, 
 FS373_3M, FS373_6M, FS373_9M, FS373_12M, 
 FS374_3M, FS374_6M, FS374_9M, FS374_12M, 
 FS375_3M, FS375_6M, FS375_9M, FS375_12M, 
 FS376_3M, FS376_6M, FS376_9M, FS376_12M, 
 FS377_3M, FS377_6M, FS377_9M, FS377_12M, 
 FS378_3M, FS378_6M, FS378_9M, FS378_12M, 
 FS379_3M, FS379_6M, FS379_9M, FS379_12M, 
 FS380_3M, FS380_6M, FS380_9M, FS380_12M, 
 FS381_3M, FS381_6M, FS381_9M, FS381_12M, 
 FS382_3M, FS382_6M, FS382_9M, FS382_12M, 
 FS383_3M, FS383_6M, FS383_9M, FS383_12M, 
 FS384_3M, FS384_6M, FS384_9M, FS384_12M, 
 FS385_3M, FS385_6M, FS385_9M, FS385_12M, 
 FS386_3M, FS386_6M, FS386_9M, FS386_12M, 
 FS387_3M, FS387_6M, FS387_9M, FS387_12M, 
 FS388_3M, FS388_6M, FS388_9M, FS388_12M, 
 FS389_3M, FS389_6M, FS389_9M, FS389_12M, 
 FS390_3M, FS390_6M, FS390_9M, FS390_12M, 
 FS391_3M, FS391_6M, FS391_9M, FS391_12M, 
 FS392_3M, FS392_6M, FS392_9M, FS392_12M,
 FS393_3M, FS393_6M, FS393_9M, FS393_12M,
 FS394_3M, FS394_6M, FS394_9M, FS394_12M,
 FS395_3M, FS395_6M, FS395_9M, FS395_12M,
 FS396_3M, FS396_6M, FS396_9M, FS396_12M,
 FS397_3M, FS397_6M, FS397_9M, FS397_12M,
 FS398_3M, FS398_6M, FS398_9M, FS398_12M,
 FS399_3M, FS399_6M, FS399_9M, FS399_12M,
 FS400_3M, FS400_6M, FS400_9M, FS400_12M,
 FS401_3M, FS401_6M, FS401_9M, FS401_12M,
 FS402_3M, FS402_6M, FS402_9M, FS402_12M,
 FS403_3M, FS403_6M, FS403_9M, FS403_12M,
 FS404_3M, FS404_6M, FS404_9M, FS404_12M,
 FS405_3M, FS405_6M, FS405_9M, FS405_12M,
 FS406_3M, FS406_6M, FS406_9M, FS406_12M,
 FS407_3M, FS407_6M, FS407_9M, FS407_12M,
 FS408_3M, FS408_6M, FS408_9M, FS408_12M,
 FS409_3M, FS409_6M, FS409_9M, FS409_12M,
 FS410_3M, FS410_6M, FS410_9M, FS410_12M,
 FS411_3M, FS411_6M, FS411_9M, FS411_12M,
 FS412_3M, FS412_6M, FS412_9M, FS412_12M,
 FS413_3M, FS413_6M, FS413_9M, FS413_12M,
 FS414_3M, FS414_6M, FS414_9M, FS414_12M,
 FS415_3M, FS415_6M, FS415_9M, FS415_12M,
 FS416_3M, FS416_6M, FS416_9M, FS416_12M,
 FS417_3M, FS417_6M, FS417_9M, FS417_12M,
 FS418_3M, FS418_6M, FS418_9M, FS418_12M,
 FS419_3M, FS419_6M, FS419_9M, FS419_12M,
 FS420_3M, FS420_6M, FS420_9M, FS420_12M,
 FS421_3M, FS421_6M, FS421_9M, FS421_12M,
 FS422_3M, FS422_6M, FS422_9M, FS422_12M,
 FS423_3M, FS423_6M, FS423_9M, FS423_12M,
 FS424_3M, FS424_6M, FS424_9M, FS424_12M,
 FS425_3M, FS425_6M, FS425_9M, FS425_12M,
 FS426_3M, FS426_6M, FS426_9M, FS426_12M,
 FS427_3M, FS427_6M, FS427_9M, FS427_12M,
 FS428_3M, FS428_6M, FS428_9M, FS428_12M,
 FS429_3M, FS429_6M, FS429_9M, FS429_12M,
 FS430_3M, FS430_6M, FS430_9M, FS430_12M,
 FS431_3M, FS431_6M, FS431_9M, FS431_12M,
 FS432_3M, FS432_6M, FS432_9M, FS432_12M,
 FS433_3M, FS433_6M, FS433_9M, FS433_12M,
 FS434_3M, FS434_6M, FS434_9M, FS434_12M,
 FS435_3M, FS435_6M, FS435_9M, FS435_12M,
 FS436_3M, FS436_6M, FS436_9M, FS436_12M,
 FS437_3M, FS437_6M, FS437_9M, FS437_12M,
 FS438_3M, FS438_6M, FS438_9M, FS438_12M,
 FS439_3M, FS439_6M, FS439_9M, FS439_12M,
 FS440_3M, FS440_6M, FS440_9M, FS440_12M,
 FS441_3M, FS441_6M, FS441_9M, FS441_12M,
 FS442_3M, FS442_6M, FS442_9M, FS442_12M,
 FS443_3M, FS443_6M, FS443_9M, FS443_12M,
 FS444_3M, FS444_6M, FS444_9M, FS444_12M,
 FS445_3M, FS445_6M, FS445_9M, FS445_12M
FROM BAM085_FS_200304
GO

INSERT INTO MASTER_BAM085_3
(IDN,
 FS446_3M, FS446_6M, FS446_9M, FS446_12M,
 FS447_3M, FS447_6M, FS447_9M, FS447_12M,
 FS448_3M, FS448_6M, FS448_9M, FS448_12M,
 FS449_3M, FS449_6M, FS449_9M, FS449_12M,
 FS450_3M, FS450_6M, FS450_9M, FS450_12M, 
 FS451_3M, FS451_6M, FS451_9M, FS451_12M,
 FS452_3M, FS452_6M, FS452_9M, FS452_12M,
 FS453_3M, FS453_6M, FS453_9M, FS453_12M,
 FS454_3M, FS454_6M, FS454_9M, FS454_12M,
 FS455_3M, FS455_6M, FS455_9M, FS455_12M,
 FS456_3M, FS456_6M, FS456_9M, FS456_12M,
 FS457_3M, FS457_6M, FS457_9M, FS457_12M,
 FS458_3M, FS458_6M, FS458_9M, FS458_12M,
 FS459_3M, FS459_6M, FS459_9M, FS459_12M,
 FS460_3M, FS460_6M, FS460_9M, FS460_12M,
 FS461_3M, FS461_6M, FS461_9M, FS461_12M,
 FS462_3M, FS462_6M, FS462_9M, FS462_12M,
 FS463_3M, FS463_6M, FS463_9M, FS463_12M,
 FS464_3M, FS464_6M, FS464_9M, FS464_12M,
 FS465_3M, FS465_6M, FS465_9M, FS465_12M,
 FS466_3M, FS466_6M, FS466_9M, FS466_12M,
 FS467_3M, FS467_6M, FS467_9M, FS467_12M,
 FS468_3M, FS468_6M, FS468_9M, FS468_12M,
 FS469_3M, FS469_6M, FS469_9M, FS469_12M,
 FS470_3M, FS470_6M, FS470_9M, FS470_12M,
 FS471_3M, FS471_6M, FS471_9M, FS471_12M,
 FS472_3M, FS472_6M, FS472_9M, FS472_12M,
 FS473_3M, FS473_6M, FS473_9M, FS473_12M,
 FS474_3M, FS474_6M, FS474_9M, FS474_12M,
 FS475_3M, FS475_6M, FS475_9M, FS475_12M,
 FS476_3M, FS476_6M, FS476_9M, FS476_12M,
 FS477_3M, FS477_6M, FS477_9M, FS477_12M,
 FS478_3M, FS478_6M, FS478_9M, FS478_12M,
 FS479_3M, FS479_6M, FS479_9M, FS479_12M,
 FS480_3M, FS480_6M, FS480_9M, FS480_12M,
 FS481_3M, FS481_6M, FS481_9M, FS481_12M,
 FS482_3M, FS482_6M, FS482_9M, FS482_12M,
 FS483_3M, FS483_6M, FS483_9M, FS483_12M,
 FS484_3M, FS484_6M, FS484_9M, FS484_12M,
 FS485_3M, FS485_6M, FS485_9M, FS485_12M,
 FS486_3M, FS486_6M, FS486_9M, FS486_12M,
 FS487_3M, FS487_6M, FS487_9M, FS487_12M,
 FS488_3M, FS488_6M, FS488_9M, FS488_12M,
 FS489_3M, FS489_6M, FS489_9M, FS489_12M,
 FS490_3M, FS490_6M, FS490_9M, FS490_12M,
 FS491_3M, FS491_6M, FS491_9M, FS491_12M,
 FS492_3M, FS492_6M, FS492_9M, FS492_12M,
 FS493_3M, FS493_6M, FS493_9M, FS493_12M,
 FS494_3M, FS494_6M, FS494_9M, FS494_12M,
 FS495_3M, FS495_6M, FS495_9M, FS495_12M,
 FS496_3M, FS496_6M, FS496_9M, FS496_12M,
 FS497_3M, FS497_6M, FS497_9M, FS497_12M,
 FS498_3M, FS498_6M, FS498_9M, FS498_12M,
 FS499_3M, FS499_6M, FS499_9M, FS499_12M,
 FS500_3M, FS500_6M, FS500_9M, FS500_12M,
 FS501_3M, FS501_6M, FS501_9M, FS501_12M,
 FS502_3M, FS502_6M, FS502_9M, FS502_12M,
 FS503_3M, FS503_6M, FS503_9M, FS503_12M,
 FS504_3M, FS504_6M, FS504_9M, FS504_12M,
 FS505_3M, FS505_6M, FS505_9M, FS505_12M,
 FS506_3M, FS506_6M, FS506_9M, FS506_12M,
 FS507_3M, FS507_6M, FS507_9M, FS507_12M,
 FS508_3M, FS508_6M, FS508_9M, FS508_12M,
 FS509_3M, FS509_6M, FS509_9M, FS509_12M,
 FS510_3M, FS510_6M, FS510_9M, FS510_12M,
 FS511_3M, FS511_6M, FS511_9M, FS511_12M,
 FS512_3M, FS512_6M, FS512_9M, FS512_12M,
 FS513_3M, FS513_6M, FS513_9M, FS513_12M,
 FS514_3M, FS514_6M, FS514_9M, FS514_12M,
 FS515_3M, FS515_6M, FS515_9M, FS515_12M,
 FS516_3M, FS516_6M, FS516_9M, FS516_12M,
 FS517_3M, FS517_6M, FS517_9M, FS517_12M,
 FS518_3M, FS518_6M, FS518_9M, FS518_12M,
 FS519_3M, FS519_6M, FS519_9M, FS519_12M,
 FS520_3M, FS520_6M, FS520_9M, FS520_12M,
 FS521_3M, FS521_6M, FS521_9M, FS521_12M,
 FS522_3M, FS522_6M, FS522_9M, FS522_12M,
 FS523_3M, FS523_6M, FS523_9M, FS523_12M,
 FS524_3M, FS524_6M, FS524_9M, FS524_12M,
 FS525_3M, FS525_6M, FS525_9M, FS525_12M,
 FS526_3M, FS526_6M, FS526_9M, FS526_12M,
 FS527_3M, FS527_6M, FS527_9M, FS527_12M,
 FS528_3M, FS528_6M, FS528_9M, FS528_12M,
 FS529_3M, FS529_6M, FS529_9M, FS529_12M,
 FS530_3M, FS530_6M, FS530_9M, FS530_12M,
 FS531_3M, FS531_6M, FS531_9M, FS531_12M,
 FS532_3M, FS532_6M, FS532_9M, FS532_12M,
 FS533_3M, FS533_6M, FS533_9M, FS533_12M,
 FS534_3M, FS534_6M, FS534_9M, FS534_12M,
 FS535_3M, FS535_6M, FS535_9M, FS535_12M,
 FS536_3M, FS536_6M, FS536_9M, FS536_12M,
 FS537_3M, FS537_6M, FS537_9M, FS537_12M,
 FS538_3M, FS538_6M, FS538_9M, FS538_12M,
 FS539_3M, FS539_6M, FS539_9M, FS539_12M,
 FS540_3M, FS540_6M, FS540_9M, FS540_12M)
SELECT
 IDN, 
 FS446_3M, FS446_6M, FS446_9M, FS446_12M,
 FS447_3M, FS447_6M, FS447_9M, FS447_12M,
 FS448_3M, FS448_6M, FS448_9M, FS448_12M,
 FS449_3M, FS449_6M, FS449_9M, FS449_12M,
 FS450_3M, FS450_6M, FS450_9M, FS450_12M, 
 FS451_3M, FS451_6M, FS451_9M, FS451_12M,
 FS452_3M, FS452_6M, FS452_9M, FS452_12M,
 FS453_3M, FS453_6M, FS453_9M, FS453_12M,
 FS454_3M, FS454_6M, FS454_9M, FS454_12M,
 FS455_3M, FS455_6M, FS455_9M, FS455_12M,
 FS456_3M, FS456_6M, FS456_9M, FS456_12M,
 FS457_3M, FS457_6M, FS457_9M, FS457_12M,
 FS458_3M, FS458_6M, FS458_9M, FS458_12M,
 FS459_3M, FS459_6M, FS459_9M, FS459_12M,
 FS460_3M, FS460_6M, FS460_9M, FS460_12M,
 FS461_3M, FS461_6M, FS461_9M, FS461_12M,
 FS462_3M, FS462_6M, FS462_9M, FS462_12M,
 FS463_3M, FS463_6M, FS463_9M, FS463_12M,
 FS464_3M, FS464_6M, FS464_9M, FS464_12M,
 FS465_3M, FS465_6M, FS465_9M, FS465_12M,
 FS466_3M, FS466_6M, FS466_9M, FS466_12M,
 FS467_3M, FS467_6M, FS467_9M, FS467_12M,
 FS468_3M, FS468_6M, FS468_9M, FS468_12M,
 FS469_3M, FS469_6M, FS469_9M, FS469_12M,
 FS470_3M, FS470_6M, FS470_9M, FS470_12M,
 FS471_3M, FS471_6M, FS471_9M, FS471_12M,
 FS472_3M, FS472_6M, FS472_9M, FS472_12M,
 FS473_3M, FS473_6M, FS473_9M, FS473_12M,
 FS474_3M, FS474_6M, FS474_9M, FS474_12M,
 FS475_3M, FS475_6M, FS475_9M, FS475_12M,
 FS476_3M, FS476_6M, FS476_9M, FS476_12M,
 FS477_3M, FS477_6M, FS477_9M, FS477_12M,
 FS478_3M, FS478_6M, FS478_9M, FS478_12M,
 FS479_3M, FS479_6M, FS479_9M, FS479_12M,
 FS480_3M, FS480_6M, FS480_9M, FS480_12M,
 FS481_3M, FS481_6M, FS481_9M, FS481_12M,
 FS482_3M, FS482_6M, FS482_9M, FS482_12M,
 FS483_3M, FS483_6M, FS483_9M, FS483_12M,
 FS484_3M, FS484_6M, FS484_9M, FS484_12M,
 FS485_3M, FS485_6M, FS485_9M, FS485_12M,
 FS486_3M, FS486_6M, FS486_9M, FS486_12M,
 FS487_3M, FS487_6M, FS487_9M, FS487_12M,
 FS488_3M, FS488_6M, FS488_9M, FS488_12M,
 FS489_3M, FS489_6M, FS489_9M, FS489_12M,
 FS490_3M, FS490_6M, FS490_9M, FS490_12M,
 FS491_3M, FS491_6M, FS491_9M, FS491_12M,
 FS492_3M, FS492_6M, FS492_9M, FS492_12M,
 FS493_3M, FS493_6M, FS493_9M, FS493_12M,
 FS494_3M, FS494_6M, FS494_9M, FS494_12M,
 FS495_3M, FS495_6M, FS495_9M, FS495_12M,
 FS496_3M, FS496_6M, FS496_9M, FS496_12M,
 FS497_3M, FS497_6M, FS497_9M, FS497_12M,
 FS498_3M, FS498_6M, FS498_9M, FS498_12M,
 FS499_3M, FS499_6M, FS499_9M, FS499_12M,
 FS500_3M, FS500_6M, FS500_9M, FS500_12M,
 FS501_3M, FS501_6M, FS501_9M, FS501_12M,
 FS502_3M, FS502_6M, FS502_9M, FS502_12M,
 FS503_3M, FS503_6M, FS503_9M, FS503_12M,
 FS504_3M, FS504_6M, FS504_9M, FS504_12M,
 FS505_3M, FS505_6M, FS505_9M, FS505_12M,
 FS506_3M, FS506_6M, FS506_9M, FS506_12M,
 FS507_3M, FS507_6M, FS507_9M, FS507_12M,
 FS508_3M, FS508_6M, FS508_9M, FS508_12M,
 FS509_3M, FS509_6M, FS509_9M, FS509_12M,
 FS510_3M, FS510_6M, FS510_9M, FS510_12M,
 FS511_3M, FS511_6M, FS511_9M, FS511_12M,
 FS512_3M, FS512_6M, FS512_9M, FS512_12M,
 FS513_3M, FS513_6M, FS513_9M, FS513_12M,
 FS514_3M, FS514_6M, FS514_9M, FS514_12M,
 FS515_3M, FS515_6M, FS515_9M, FS515_12M,
 FS516_3M, FS516_6M, FS516_9M, FS516_12M,
 FS517_3M, FS517_6M, FS517_9M, FS517_12M,
 FS518_3M, FS518_6M, FS518_9M, FS518_12M,
 FS519_3M, FS519_6M, FS519_9M, FS519_12M,
 FS520_3M, FS520_6M, FS520_9M, FS520_12M,
 FS521_3M, FS521_6M, FS521_9M, FS521_12M,
 FS522_3M, FS522_6M, FS522_9M, FS522_12M,
 FS523_3M, FS523_6M, FS523_9M, FS523_12M,
 FS524_3M, FS524_6M, FS524_9M, FS524_12M,
 FS525_3M, FS525_6M, FS525_9M, FS525_12M,
 FS526_3M, FS526_6M, FS526_9M, FS526_12M,
 FS527_3M, FS527_6M, FS527_9M, FS527_12M,
 FS528_3M, FS528_6M, FS528_9M, FS528_12M,
 FS529_3M, FS529_6M, FS529_9M, FS529_12M,
 FS530_3M, FS530_6M, FS530_9M, FS530_12M,
 FS531_3M, FS531_6M, FS531_9M, FS531_12M,
 FS532_3M, FS532_6M, FS532_9M, FS532_12M,
 FS533_3M, FS533_6M, FS533_9M, FS533_12M,
 FS534_3M, FS534_6M, FS534_9M, FS534_12M,
 FS535_3M, FS535_6M, FS535_9M, FS535_12M,
 FS536_3M, FS536_6M, FS536_9M, FS536_12M,
 FS537_3M, FS537_6M, FS537_9M, FS537_12M,
 FS538_3M, FS538_6M, FS538_9M, FS538_12M,
 FS539_3M, FS539_6M, FS539_9M, FS539_12M,
 FS540_3M, FS540_6M, FS540_9M, FS540_12M
FROM BAM085_FS_200304
GO

INSERT INTO MASTER_BAM085_4
(IDN,
 FS541_3M, FS541_6M, FS541_9M, FS541_12M,
 FS542_3M, FS542_6M, FS542_9M, FS542_12M,
 FS543_3M, FS543_6M, FS543_9M, FS543_12M,
 FS544_3M, FS544_6M, FS544_9M, FS544_12M,
 FS545_3M, FS545_6M, FS545_9M, FS545_12M,
 FS546_3M, FS546_6M, FS546_9M, FS546_12M,
 FS547_3M, FS547_6M, FS547_9M, FS547_12M,
 FS548_3M, FS548_6M, FS548_9M, FS548_12M,
 FS549_3M, FS549_6M, FS549_9M, FS549_12M,
 FS550_3M, FS550_6M, FS550_9M, FS550_12M,
 FS551_3M, FS551_6M, FS551_9M, FS551_12M,
 FS552_3M, FS552_6M, FS552_9M, FS552_12M,
 FS553_3M, FS553_6M, FS553_9M, FS553_12M,
 FS554_3M, FS554_6M, FS554_9M, FS554_12M)
SELECT
 IDN,
 FS541_3M, FS541_6M, FS541_9M, FS541_12M,
 FS542_3M, FS542_6M, FS542_9M, FS542_12M,
 FS543_3M, FS543_6M, FS543_9M, FS543_12M,
 FS544_3M, FS544_6M, FS544_9M, FS544_12M,
 FS545_3M, FS545_6M, FS545_9M, FS545_12M,
 FS546_3M, FS546_6M, FS546_9M, FS546_12M,
 FS547_3M, FS547_6M, FS547_9M, FS547_12M,
 FS548_3M, FS548_6M, FS548_9M, FS548_12M,
 FS549_3M, FS549_6M, FS549_9M, FS549_12M,
 FS550_3M, FS550_6M, FS550_9M, FS550_12M,
 FS551_3M, FS551_6M, FS551_9M, FS551_12M,
 FS552_3M, FS552_6M, FS552_9M, FS552_12M,
 FS553_3M, FS553_6M, FS553_9M, FS553_12M,
 FS554_3M, FS554_6M, FS554_9M, FS554_12M
FROM BAM085_FS_200304
GO

INSERT INTO MASTER_BAM101_1
(IDN,
 MS405_3M, MS405_6M, MS405_9M, MS405_12M,
 MS406_3M, MS406_6M, MS406_9M, MS406_12M,
 MS407_3M, MS407_6M, MS407_9M, MS407_12M,
 MS408_3M, MS408_6M, MS408_9M, MS408_12M,
 MS409_3M, MS409_6M, MS409_9M, MS409_12M,
 MS410_3M, MS410_6M, MS410_9M, MS410_12M,
 MS411_3M, MS411_6M, MS411_9M, MS411_12M,
 MS412_3M, MS412_6M, MS412_9M, MS412_12M,
 MS413_3M, MS413_6M, MS413_9M, MS413_12M,
 MS414_3M, MS414_6M, MS414_9M, MS414_12M,
 MS415_3M, MS415_6M, MS415_9M, MS415_12M,
 MS416_3M, MS416_6M, MS416_9M, MS416_12M,
 MS417_3M, MS417_6M, MS417_9M, MS417_12M,
 MS418_3M, MS418_6M, MS418_9M, MS418_12M,
 MS419_3M, MS419_6M, MS419_9M, MS419_12M,
 MS420_3M, MS420_6M, MS420_9M, MS420_12M,
 MS421_3M, MS421_6M, MS421_9M, MS421_12M,
 MS422_3M, MS422_6M, MS422_9M, MS422_12M,
 MS423_3M, MS423_6M, MS423_9M, MS423_12M,
 MS424_3M, MS424_6M, MS424_9M, MS424_12M,
 MS425_3M, MS425_6M, MS425_9M, MS425_12M,
 MS426_3M, MS426_6M, MS426_9M, MS426_12M)
SELECT
IDN,
 MS405_3M, MS405_6M, MS405_9M, MS405_12M,
 MS406_3M, MS406_6M, MS406_9M, MS406_12M,
 MS407_3M, MS407_6M, MS407_9M, MS407_12M,
 MS408_3M, MS408_6M, MS408_9M, MS408_12M,
 MS409_3M, MS409_6M, MS409_9M, MS409_12M,
 MS410_3M, MS410_6M, MS410_9M, MS410_12M,
 MS411_3M, MS411_6M, MS411_9M, MS411_12M,
 MS412_3M, MS412_6M, MS412_9M, MS412_12M,
 MS413_3M, MS413_6M, MS413_9M, MS413_12M,
 MS414_3M, MS414_6M, MS414_9M, MS414_12M,
 MS415_3M, MS415_6M, MS415_9M, MS415_12M,
 MS416_3M, MS416_6M, MS416_9M, MS416_12M,
 MS417_3M, MS417_6M, MS417_9M, MS417_12M,
 MS418_3M, MS418_6M, MS418_9M, MS418_12M,
 MS419_3M, MS419_6M, MS419_9M, MS419_12M,
 MS420_3M, MS420_6M, MS420_9M, MS420_12M,
 MS421_3M, MS421_6M, MS421_9M, MS421_12M,
 MS422_3M, MS422_6M, MS422_9M, MS422_12M,
 MS423_3M, MS423_6M, MS423_9M, MS423_12M,
 MS424_3M, MS424_6M, MS424_9M, MS424_12M,
 MS425_3M, MS425_6M, MS425_9M, MS425_12M,
 MS426_3M, MS426_6M, MS426_9M, MS426_12M 
FROM BAM101_MS_200304
GO