namespace CS;

public partial class SettingsDialog : Form
{
    TextBox host;
    Label hostLabel;
    TextBox port;
    Label portLabel;
    TextBox username;
    Label usernameLabel;
    TextBox password;
    Label passwordLabel;
    TextBox database;
    Label databaseLabel;

    Button okButton;
    Button cancelButton;
    public SettingsDialog(string shost, string sport, string susername,
    string spassword, string sdatabase){
        this.Text = "Настройки";
        

        this.MaximumSize = new Size(230, 220);
        this.MinimumSize = this.MaximumSize;

        TableLayoutPanel layout = new TableLayoutPanel();
        layout.RowCount = 6;
        layout.ColumnCount = 2;
        layout.Dock = DockStyle.Fill;
        layout.Margin = new Padding(10);
        this.Controls.Add(layout);

        int row = 0;

        hostLabel = new Label();
        hostLabel.Text = "Сервер:";

        host = new TextBox();

        layout.Controls.Add(hostLabel, 0, row);
        layout.Controls.Add(host, 1, row++);


        portLabel = new Label();
        portLabel.Text = "Порт:";

        port = new TextBox();

        layout.Controls.Add(portLabel, 0, row);
        layout.Controls.Add(port, 1, row++);


        usernameLabel = new Label();
        usernameLabel.Text = "Имя пользователя:";

        username = new TextBox();

        layout.Controls.Add(usernameLabel, 0, row);
        layout.Controls.Add(username, 1, row++);


        passwordLabel = new Label();
        passwordLabel.Text = "Пароль:";

        password = new TextBox();

        layout.Controls.Add(passwordLabel, 0, row);
        layout.Controls.Add(password, 1, row++);

        databaseLabel = new Label();
        databaseLabel.Text = "База данных:";

        database = new TextBox();

        layout.Controls.Add(databaseLabel, 0, row);
        layout.Controls.Add(database, 1, row++);

        this.host.Text = shost;
        this.port.Text = sport;
        this.username.Text = susername;
        this.password.Text = spassword;
        this.database.Text = sdatabase;

        okButton = new Button();
        okButton.Text = "Принять";
        okButton.Click += new EventHandler(okButton_Click);

        cancelButton = new Button();
        cancelButton.Text = "Отмена";
        cancelButton.Click += new EventHandler(cancelButton_Click);

        layout.Controls.Add(okButton, 0, row);
        layout.Controls.Add(cancelButton, 1, row++);
    }

    private void okButton_Click(object sender, EventArgs e){
        this.DialogResult = DialogResult.OK;
        this.Close();
    }

    private void cancelButton_Click(object sender, EventArgs e){
        this.DialogResult = DialogResult.Cancel;
        this.Close();
    }

    public string Host{
        get { return this.host.Text; }
    }

    public string Port{
        get { return this.port.Text; }
    }

    public string Username{
        get { return this.username.Text; }
    }

    public string Password{
        get { return this.password.Text; }
    }

    public string Database{
        get { return this.database.Text; }
    }
}