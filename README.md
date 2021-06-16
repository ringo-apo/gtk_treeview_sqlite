# gtk_treeview_sqlite
# コンパイル

```
gcc -Wall main.c `pkg-config --cflags --libs gtk+-3.0` -lsqlite3 -L/usr/lib64/ -I/usr/include/
```

# 実行

```
./a.out
```
