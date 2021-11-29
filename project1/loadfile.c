#define WORD_MAX_LEN 200
#define MEAN_MAX_LEN 40000
#define LINE_MAX_LEN 40000

// Load file dữ liệu
void loadFile(char *fileName)
{
    openBT();

    // Set chuỗi "Loading dictionary...\n" cho meaningViewBuff
    gtk_text_buffer_set_text(meaningViewBuff, "Loading dictionary...\n", -1);

    int wordCount = 0; // đếm số từ insert thành công vào btree file

    char *tmp_word;
    char *tmp_meaning;
    char *line;
    char *linePtr;

    char notify[MEAN_MAX_LEN]; // Chuỗi thông báo

    char *word;
    char *meaning;
    char wordLower[WORD_MAX_LEN];

    tmp_word = malloc(WORD_MAX_LEN * sizeof(char));
    tmp_meaning = malloc(MEAN_MAX_LEN * sizeof(char));

    word = malloc(WORD_MAX_LEN * sizeof(char));
    meaning = malloc(MEAN_MAX_LEN * sizeof(char));

    line = malloc(LINE_MAX_LEN * sizeof(char));

    int i;
    BTint j;
    FILE *f;
    if ((f = fopen(fileName, "r")) == NULL)
    {
        gtk_text_buffer_insert_at_cursor(meaningViewBuff, "File open failed!\n", -1);
        return;
    }
    else
    {
        // Find the first word

        // Read a line in dict file
        linePtr = fgets(line, LINE_MAX_LEN, f);

        while (linePtr != NULL)
        {

            if (feof(f))
            {
                return;
            }

            if (strlen(line) == 0)
            {
                // Read a line in dict file
                linePtr = fgets(line, LINE_MAX_LEN, f);
                continue;
            }

            // Split word
            for (i = 1; line[i] != '/' && line[i] != '\n' && line[i] != '\0'; i++)
                ;
            strncpy(tmp_word, &line[1], i - 1);
            tmp_word[i - 1] = '\0';

            // Split meaning
            strncpy(tmp_meaning, &line[i], strlen(line) - i + 1);

            // Continuously read lines containing meaning
            // Read a line in dict file
            linePtr = fgets(line, LINE_MAX_LEN, f);
            while (linePtr != NULL && line[0] != '@')
            {
                if (strlen(tmp_meaning) + strlen(line) > MEAN_MAX_LEN)
                {
                    sprintf(notify, "Meaning exceeded the maximum length. Word: %S\n", tmp_word);
                    gtk_text_buffer_insert_at_cursor(meaningViewBuff, notify, -1);
                    exit(1);
                }
                strcat(tmp_meaning, line);
                // Read a line in dict file
                linePtr = fgets(line, LINE_MAX_LEN, f);
            }

            strcpy(word, tmp_word);
            strcpy(meaning, tmp_meaning);

            strLower(wordLower, word); // convert word to lower
            chuanHoa(wordLower);       // trim wordLower
            chuanHoa(meaning);

            if (bfndky(eng_vie, wordLower, &j) != 0)
            {
                btins(eng_vie, wordLower, meaning, strlen(meaning) * sizeof(char)); // Insert word to Btree
                wordCount++;                                                        // Increase wordCount
            }
        }
    }

    fclose(f);
    buildListSuggest(searchEntry);     // Cập nhật lại đề xuất cho ô tìm kiếm
    buildListSuggest(entryDeleteWord); //Cập nhật lại đề xuất cho ô xóa

    //Hàm sprintf gần giống với các hàm printf, fprintf nhưng nó không in ra stdout, file mà nó "in" vào chuỗi
    sprintf(notify, "Loading done. %d words was loaded.", wordCount);

    //Set buffer cho meaningViewBuff là notify ở trên
    gtk_text_buffer_insert_at_cursor(meaningViewBuff, notify, strlen(notify));

    //free used memory
    free(word);
    free(meaning);
    free(tmp_word);
    free(tmp_meaning);
    free(line);

    // Save the dictionary by re-opening
    sprintf(notify, "\nSaving file...\n");

    closeBT();

    //Append notify to meaningViewBuff
    gtk_text_buffer_insert_at_cursor(meaningViewBuff, notify, strlen(notify));
    gtk_text_buffer_insert_at_cursor(meaningViewBuff, "Loading done. Use search entry to lookup words.", -1);
}


// Hàm xử lý nhấn phím Enter (Tìm từ)
void on_searchEntry_activate(GtkEntry *searchEntry, gpointer user_data)
{
    gchar *word = gtk_entry_buffer_get_text(gtk_entry_get_buffer(searchEntry));

    chuanHoa(word);

    if (strlen(word) <= 0)
    {
        messageDialog("Vui long nhap day du!!!", 1);
    }
    else
    {
        lookUp(word); // Tìm từ nếu strlen(word) > 0
    }
}

// Hàm xử lý sự kiện bấm phím ở searchEntry
gboolean onEventKeyInSearchEntry(GtkWidget *entry, GdkEventKey *key, gpointer data)
{
    //Kiểm tra xem có phải đang nhấn nút hay không
    if (key)
    {
        //Kiểm tra xem nút nhấn có phải là Tab hay không
        if (key->keyval == GDK_KEY_Tab)
        {
            //Khi ấn Tab ở searchEntry sẽ gọi hàm tự động hoàn thành từ (giống như trong Terminal)
            autoComplete(GTK_ENTRY(entry));
        }
        //Kiểm tra xem nút nhấn có phải là nút Backspace hay Delete hay không
        else if (key->keyval == GDK_KEY_BackSpace || key->keyval == GDK_KEY_Delete)
        {
            // Khi xóa từ sẽ đặt rỗng cho meaningViewBuff
            gtk_text_buffer_set_text(meaningViewBuff, "", -1);
        }
    }

    //Phải trả về là false thì searchEntry mới có thể hiện chữ được. Còn trả về là TRUE thì không hiện chữ được
    return FALSE;
}
