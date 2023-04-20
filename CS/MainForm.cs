using Npgsql;
using System.Data;
using System.Configuration;

namespace CS;

public partial class MainForm : Form
{
    string host;
    string port;
    string username;
    string password;
    string database;
    TabControl tableTabs;

    TableLayoutPanel mainLayout;

    MenuStrip mainMenu;
    ToolStripMenuItem connectMenuItem;
    ToolStripMenuItem disconnectMenuItem;
    ToolStripMenuItem connectionSettingsMenuItem;
    ToolStripMenuItem queryesMenuItem;
    ToolStripMenuItem searchInnMenuItem;
    ToolStripMenuItem callProcMenuItem;

    object oldValue;
    int oldRow;
    int oldColumn;
    public MainForm()
    {
        this.Text = "Лабораторная работа №6";

        InitializeComponent();
        host = ConfigurationManager.AppSettings["host"];
        port = ConfigurationManager.AppSettings["port"];
        username = ConfigurationManager.AppSettings["username"];
        password = ConfigurationManager.AppSettings["password"];
        database = ConfigurationManager.AppSettings["database"];
        initializeLayout();
        initializeMenuItems();
        this.tableTabs = new TabControl();
        this.tableTabs.Dock = DockStyle.Fill;
        mainLayout.Controls.Add(this.tableTabs, 0, 1);
    }

    protected void initializeLayout(){
        mainLayout = new TableLayoutPanel();
        mainLayout.RowCount = 2;
        mainLayout.ColumnCount = 1;
        mainLayout.Dock = DockStyle.Fill;
        mainLayout.Margin = new Padding(10);
        this.Controls.Add(mainLayout);
    }

    protected void initializeMenuItems(){
        mainMenu = new MenuStrip();
        mainLayout.Controls.Add(mainMenu, 0, 0);

        connectMenuItem = new ToolStripMenuItem("Подключиться");
        connectMenuItem.Click += new EventHandler(connectMenuItem_Click);
        mainMenu.Items.Add(connectMenuItem);

        disconnectMenuItem = new ToolStripMenuItem("Отключиться");
        disconnectMenuItem.Click += new EventHandler(disconnectMenuItem_Click);
        mainMenu.Items.Add(disconnectMenuItem);
        disconnectMenuItem.Enabled = false;

        connectionSettingsMenuItem = new ToolStripMenuItem("Настройки подключения");
        connectionSettingsMenuItem.Click += new EventHandler(connectionSettings_Click);
        mainMenu.Items.Add(connectionSettingsMenuItem);

        queryesMenuItem = new ToolStripMenuItem("Запросы");
        mainMenu.Items.Add(queryesMenuItem);
        queryesMenuItem.Enabled = false;

        searchInnMenuItem = new ToolStripMenuItem("Поиск по ИНН");
        searchInnMenuItem.Click += new EventHandler(searchInnMenuItem_Click);
        queryesMenuItem.DropDownItems.Add(searchInnMenuItem);

        callProcMenuItem = new ToolStripMenuItem("Процедура вставки");
        callProcMenuItem.Click += new EventHandler(callProcMenuItem_Click);
        queryesMenuItem.DropDownItems.Add(callProcMenuItem);
    }

    protected string connectString(){
        string connectString = $"Host={host};";
        connectString += $"Port={port};";
        connectString += $"Username={username};";
        connectString += $"Password={password};";
        connectString += $"Database={database}";
        return connectString;
    }

    protected NpgsqlConnection newConnection(){
        string connString = connectString();
        return new NpgsqlConnection(connString);
    }

    protected List<string> getTableNames(){
        List<string> tables = new List<string>();

        using (var conn = newConnection()){
            try{
                conn.Open();
                string query = "SELECT table_name "+
                "FROM information_schema.tables WHERE table_schema = 'public' "+
                "ORDER BY table_name";
                NpgsqlCommand command = new NpgsqlCommand(query, conn);
                NpgsqlDataReader reader = command.ExecuteReader();
                while (reader.Read()){
                    string table = reader.GetString(0);
                    tables.Add(table);
                }
                conn.Close();
            }
            catch (Exception ex){
                throw new Exception($"Не удалось получить названия таблиц\n{ex}");
            }
        }
        return tables;
    }

    protected DataGridView getDataGridViewForTable(string table){
        DataGridView tableGridView = new DataGridView();

        using (var conn = newConnection()){
            try{
                conn.Open();
                string sql = $"SELECT * FROM {table}";
                NpgsqlDataAdapter dataAdapter = new NpgsqlDataAdapter(sql,
                                                    connectString());
                DataTable dataTable = new DataTable();
                dataAdapter.Fill(dataTable);
                tableGridView.DataSource = dataTable;
                conn.Close();
            }
            catch (Exception ex){
                throw new Exception("Не удалось получить данные"+
                $" из таблицы {table}\n{ex}\n{connectString()}");
                return tableGridView;
            }
        }
        return tableGridView;
    }

    protected void clearTables(){
        while (tableTabs.TabCount > 0){
            tableTabs.TabPages.RemoveAt(0);
        }
    }

    protected void showTables(){
        List<string> tables;
        try{
            tables = getTableNames();
        }
        catch (Exception ex){
            MessageBox.Show($"ОШИБКА: {ex}",
            "Ошибка получения данных");
            return;
        }

        foreach (string table in tables){
            TabPage tab = new TabPage(table);
            DataGridView data = new DataGridView();
            try {
                data = getDataGridViewForTable(table);
            }
            catch (Exception ex){
                MessageBox.Show($"ОШИБКА: {ex}",
                "Ошибка получения данных");
                return;
            }
            data.Name = table;
            data.CellBeginEdit += data_CellBeginEdit;
            data.CellValueChanged += data_CellValueChanged;
            tab.Controls.Add(data);
            data.Dock = DockStyle.Fill;
            tableTabs.TabPages.Add(tab);
        }
    }

    private void data_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e){
        if (e.RowIndex < 0 || e.ColumnIndex < 0) { return; }
        DataGridView dataGridView = sender as DataGridView;
        DataGridViewCell cell = dataGridView.Rows[e.RowIndex].Cells[e.ColumnIndex];
        oldValue = cell.Value;
        oldRow = e.RowIndex;
        oldColumn = e.ColumnIndex;
    }

    private void data_CellValueChanged(object sender, DataGridViewCellEventArgs e) {
        if (e.RowIndex < 0 || e.ColumnIndex < 0) { return; }
        DataGridView dataGridView = sender as DataGridView;
        DataGridViewCell cell = dataGridView.Rows[e.RowIndex].Cells[e.ColumnIndex];
        object value = cell.Value;

        string sql = $"UPDATE {dataGridView.Name} SET ";
        string condition = " WHERE ";

        for (int i = 0; i < dataGridView.ColumnCount; i++){
            sql += dataGridView.Columns[i].HeaderText + "=";
            sql+= "'" + dataGridView.Rows[e.RowIndex].Cells[i].Value + "'";

            condition += dataGridView.Columns[i].HeaderText + "=";
            if (i != oldColumn){
                condition += "'" + dataGridView.Rows[e.RowIndex].Cells[i].Value + "'";
            }
            else {
                condition += "'" + oldValue + "'";
            }

            if (i != dataGridView.ColumnCount - 1){
                sql += ", ";
                condition += " AND ";
            }
        }
        sql += condition;
        MessageBox.Show($"{sql}","Ошибка получения данных");

        using (var conn = newConnection()){
            try{
                conn.Open();
                using var cmd = new NpgsqlCommand(sql, conn);
                cmd.ExecuteNonQuery();
                conn.Close();
            }
            catch (Exception ex){
                MessageBox.Show($"ОШИБКА: Не удалось произвести выполнение процедуры\n{ex}",
                "Ошибка выполнения процедуры");
                this.Enabled = true;
                return;         
            }
        }
        refrasheTables();
    }


    protected void refrasheTables(){
        clearTables();
        showTables();
    }

    private void connectMenuItem_Click(object sender, EventArgs e){
        showTables();
        disconnectMenuItem.Enabled = true;
        queryesMenuItem.Enabled = true;
        connectMenuItem.Enabled = false;
    }

    private void disconnectMenuItem_Click(object sender, EventArgs e){
        clearTables();
        disconnectMenuItem.Enabled = false;
        queryesMenuItem.Enabled = false;
        connectMenuItem.Enabled = true;
    }

    private void connectionSettings_Click(object sender, EventArgs e){
        SettingsDialog dialog = new SettingsDialog(host, port, username,
        password, database);
        this.Enabled = false;
        DialogResult result = dialog.ShowDialog();
        if (result == DialogResult.Cancel){
            this.Enabled = true;
            return;
        }
        Configuration config = ConfigurationManager.OpenExeConfiguration(
            ConfigurationUserLevel.None
        );
        host = dialog.Host;
        port = dialog.Port;
        username = dialog.Username;
        password = dialog.Password;
        database = dialog.Database;
        config.AppSettings.Settings["host"].Value = host;
        config.AppSettings.Settings["port"].Value = port;
        config.AppSettings.Settings["username"].Value = username;
        config.AppSettings.Settings["password"].Value = password;
        config.AppSettings.Settings["database"].Value = database;
        config.Save(ConfigurationSaveMode.Modified);
        ConfigurationManager.RefreshSection("appSettings");
        this.Enabled = true;
    }

    private void searchInnMenuItem_Click(object sender, EventArgs e){
        string inn = (Microsoft.VisualBasic.Interaction.InputBox("Введите ИНН:", "Поиск по ИНН", "")).ToString();
        
        refrasheTables();

        DataGridView tableGridView = new DataGridView();
        
        using(var conn = newConnection()){
            try{
                conn.Open();
                string sql = $"SELECT * FROM call WHERE inn='{inn}'";
                NpgsqlDataAdapter dataAdapter = new NpgsqlDataAdapter(sql,
                connectString());
                DataTable dataTable = new DataTable();
                dataAdapter.Fill(dataTable);
                tableGridView.DataSource = dataTable;
                TabPage tab = new TabPage("Результат поиска");
                tab.Controls.Add(tableGridView);
                tableGridView.Dock = DockStyle.Fill;
                tableTabs.TabPages.Add(tab);
                conn.Close();
            }
            catch (Exception ex){
                MessageBox.Show($"ОШИБКА: Не удалось произвести поиск\n{ex}",
                "Ошибка получения данных");
                return;                
            }
        }   
    }

    private void callProcMenuItem_Click(object sender, EventArgs e){
        ProcDialog dialog = new ProcDialog();
        this.Enabled = false;
        DialogResult result = dialog.ShowDialog();
        if (result == DialogResult.Cancel){
            this.Enabled = true;
            return;
        }
        string telephonePoint = dialog.TelephonePoint;
        string inn = dialog.Inn;
        string checkingAccaunt = dialog.CheckingAccaunt;
        string timesOfDay = dialog.TimesOfDay;
        string duration = dialog.Duration;
        string callDate = dialog.CallDate;
        string city = dialog.City;
        string discount = dialog.Discount;
        string priceDaytime = dialog.PriceDaytime;
        string priceNight = dialog.PriceNight;

        using (var conn = newConnection()){
            try{
                conn.Open();
                string sql = $"CALL insert_call('{telephonePoint}','{inn}',"+
                $"'{checkingAccaunt}','{timesOfDay}','{duration}','{callDate}',"+
                $"'{city}','{discount}','{priceDaytime}','{priceNight}')";
                using var cmd = new NpgsqlCommand(sql, conn);
                cmd.ExecuteNonQuery();
                conn.Close();
            }
            catch (Exception ex){
                MessageBox.Show($"ОШИБКА: Не удалось произвести выполнение процедуры\n{ex}",
                "Ошибка выполнения процедуры");
                this.Enabled = true;
                return;         
            }
        }
        this.Enabled = true;
        refrasheTables();
    }
}
