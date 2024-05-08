# XXE Bypass

## 字符编码绕过
XML 解析器使用 4 种方法来检测编码：

+ HTTP 内容类型：Content-Type: text/xml; charset=utf-8
+ 读取字节顺序标记 (BOM)
+ 读取文档的第一个符号
  + UTF-8 (3C 3F 78 6D)
  + UTF-16BE (00 3C 00 3F)
  + UTF-16LE (3C 00 3F 00)
+ XML 声明：<?xml version="1.0" encoding="UTF-8"?>
