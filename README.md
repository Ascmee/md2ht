# md2ht

由于C++更适合做这种命令行的程序，所以使用了不太擅长的C++来做这个，可能以后有空时，会用Java来写这个小项目。

本项目用于生成我的博客模板，转换markdown为适合我的博客模板的html文件。

## 缺点

由于能力有限，所以markdown文件中若是出现了太过于复杂的markdown语法嵌套，将会产生难以预料的错误

暂时对图表，脚注与YAML的markdown语法没有支持

由于还没有想到好的算法来提升程序的性能，所以效率可能会很低

## 使用

本程序采用命令行的形式来使用，虽然使得程序的操作更加的复杂，但是我认为这很符合它

使用-h或--help来获取帮助

**-o**与--**output**参数可以用来指定文章的标题，若是没有指定，则以markdown文件的名字为标题，由于**index.html**与**about.html**都采用了文章的模式，所以可以通过-**in**或-**ab**参数来指定该文件为 index.html或about.html

```example
md2ht -o [articles name] [md path]
```

```example
md2ht -in [md path]
```

```example
md2ht -ab [md path]
```

**-c**与-**classify**参数用于指定文章的分类，若是没有使用分类，则使用默认分类

```example
md2ht -c [classifiction] [md path]
```

**-pr**与--**project**用于指定该文章为项目，需要指定项目地址

```example
md2ht -pr [url] [md path]
```

因为index与about页面的没有标题与分类，所以不可以与-**o**、**-c**、**-pr**参数一起使用

**-cs**与--**classifications**用于设置所有分类，至少为4个，最多为9个，最后一个为默认分类

```example
md2ht -cs [classification 1]...[classification n]
```

**-a**与--**author**用于指定博主的名字与博主的主页地址

```example
md2ht -a [blogger name] [url]
```

**-r**与--**remove**用于删除对应的文章

```example
md2ht -r [html path]
```

**--has**用于指定博客所在的文件夹

```example
md2ht --has [blog dir path]
```



本程序会使用git命令，所以环境中要具备git，第一次使用时，推荐先指定分类等信息，程序会利用git下载模板，下载后将blog_template文件夹的名字改为你想要的名字，然后通过--**has**命令指定该文件夹



## 帮助

由于能力有限，如果可以，请帮助完善这个项目
