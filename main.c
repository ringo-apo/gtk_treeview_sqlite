#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include <gtk/gtk.h>

struct data {
    int a;
    char b[10];
};

void create_model(GtkWidget *treeview)
{
    GtkListStore *store;

    store = gtk_list_store_new(
                2,              //  要素の個数
                G_TYPE_INT,
                G_TYPE_STRING
            );

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    g_object_unref(store);
}

void set_data(GtkWidget *treeview)
{

    sqlite3 *db;
    char *filename="testdb.db";
    int rc; //result codes
    sqlite3_stmt *stmt;
    int i;
    int j = 0;
    struct data datas[100];

    rc = sqlite3_open(filename, &db);
    if(rc != SQLITE_OK){
        return 1;
    }

    //プリペアドステートメントを生成
    rc = sqlite3_prepare_v2(db,"SELECT id, name FROM testtable", -1, &stmt, 0);
    if(rc != SQLITE_OK){
        printf("ERROR(%d) %s\n",rc, sqlite3_errmsg(db));
        sqlite3_close(db);
        return 2;
    }

    //行を読み取る。
    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW){

        datas[j].a = sqlite3_column_int(stmt,0);
        strcpy(datas[j].b, sqlite3_column_text(stmt,1));

        //printf("%d %s\n", datas[j].a, datas[j].b);

        j++;

        rc = sqlite3_step(stmt);
    }

    //プレペアドステートメントの解放
    rc = sqlite3_finalize(stmt);
    if(rc != SQLITE_OK){
        printf("ERROR(%d) %s\n",rc,sqlite3_errmsg(db));
    }

    sqlite3_close(db);

    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE( gtk_tree_view_get_model( GTK_TREE_VIEW(treeview) ) );
    gtk_list_store_clear(store);

    for (int i = 0; i<=2; i++)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, datas[i].a, 1, datas[i].b, -1);
    }

}

void append_column_to_treeview(GtkWidget *treeview, const char *title, const int order)
{
    GtkTreeViewColumn   *column;
    GtkCellRenderer *renderer;

    renderer = gtk_cell_renderer_text_new();

    column = gtk_tree_view_column_new_with_attributes(
                title,          //  ヘッダ行に表示するタイトル
                renderer,       //  CellRenderer
                "text",  order, //  Model の order 番めのデータを、renderer の属性 "text" に結びつける
                NULL            //  終端
             );

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}

void create_view(GtkWidget *treeview)
{
    append_column_to_treeview(treeview, "Int", 0);
    append_column_to_treeview(treeview, "String", 1);
}


int main(int argc, char **argv)
{
    GtkWidget *window, *treeview;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "treeView");

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    treeview = gtk_tree_view_new();

    create_model(treeview);

    set_data(treeview);

    create_view(treeview);

    gtk_container_add(GTK_CONTAINER(window), treeview);

    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
