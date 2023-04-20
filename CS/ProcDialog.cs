namespace CS;

public partial class ProcDialog : Form
{
    TextBox telephonePoint;
    TextBox inn;
    TextBox checkingAccaunt;
    TextBox timesOfDay;
    TextBox duration;
    TextBox callDate;
    TextBox city;
    TextBox discount;
    TextBox priceDaytime;
    TextBox priceNight;

    Button okButton;
    Button cancelButton;

    public ProcDialog(){
        this.MaximumSize = new Size(230, 360);
        this.MinimumSize = this.MaximumSize;

        TableLayoutPanel layout = new TableLayoutPanel();
        layout.RowCount = 6;
        layout.ColumnCount = 2;
        layout.Dock = DockStyle.Fill;
        layout.Margin = new Padding(10);
        this.Controls.Add(layout);

        Label telephonePointLabel = new Label();
        telephonePointLabel.Text = "Телефонная точка: ";

        Label innLabel = new Label();
        innLabel.Text = "ИНН: ";

        Label checkingAccauntLabel = new Label();
        checkingAccauntLabel.Text = "Рассчетный счет: ";

        Label timesOfDayLabel = new Label();
        timesOfDayLabel.Text = "Время суток: ";

        Label durationLabel = new Label();
        durationLabel.Text = "Длительность: ";

        Label callDateLabel = new Label();
        callDateLabel.Text = "Дата звонка: ";

        Label cityLabel = new Label();
        cityLabel.Text = "Город: ";

        Label discountLabel = new Label();
        discountLabel.Text = "Скидка: ";

        Label priceDaytimeLabel = new Label();
        priceDaytimeLabel.Text = "Дневной тариф: ";

        Label priceNightLabel = new Label();
        priceNightLabel.Text = "Ночной тариф: ";

        telephonePoint = new TextBox();
        inn = new TextBox();
        checkingAccaunt = new TextBox();
        timesOfDay = new TextBox();
        duration = new TextBox();
        callDate = new TextBox();
        city = new TextBox();
        discount = new TextBox();
        priceDaytime = new TextBox();
        priceNight = new TextBox();

        layout.Controls.Add(telephonePointLabel, 0, 0);
        layout.Controls.Add(telephonePoint, 1, 0);

        layout.Controls.Add(innLabel, 0, 1);
        layout.Controls.Add(inn, 1, 1);

        layout.Controls.Add(checkingAccauntLabel, 0, 2);
        layout.Controls.Add(checkingAccaunt, 1, 2);

        layout.Controls.Add(timesOfDayLabel, 0, 3);
        layout.Controls.Add(timesOfDay, 1, 3);

        layout.Controls.Add(durationLabel, 0, 4);
        layout.Controls.Add(duration, 1, 4);

        layout.Controls.Add(callDateLabel, 0, 5);
        layout.Controls.Add(callDate, 1, 5);

        layout.Controls.Add(cityLabel, 0, 6);
        layout.Controls.Add(city, 1, 6);

        layout.Controls.Add(discountLabel, 0, 7);
        layout.Controls.Add(discount, 1, 7);

        layout.Controls.Add(priceDaytimeLabel, 0, 8);
        layout.Controls.Add(priceDaytime, 1, 8);

        layout.Controls.Add(priceNightLabel, 0, 9);
        layout.Controls.Add(priceNight, 1, 9);

        okButton = new Button();
        okButton.Text = "Принять";
        okButton.Click += new EventHandler(okButton_Click);

        cancelButton = new Button();
        cancelButton.Text = "Отмена";
        cancelButton.Click += new EventHandler(cancelButton_Click);

        layout.Controls.Add(okButton, 0, 10);
        layout.Controls.Add(cancelButton, 0, 10);
    }

    private void okButton_Click(object sender, EventArgs e){
        this.DialogResult = DialogResult.OK;
        this.Close();
    }

    private void cancelButton_Click(object sender, EventArgs e){
        this.DialogResult = DialogResult.Cancel;
        this.Close();
    }

    public string TelephonePoint 
    {
        get { return telephonePoint.Text; }
    }

    public string Inn 
    {
        get { return inn.Text; }
    }

    public string CheckingAccaunt 
    {
        get { return checkingAccaunt.Text; }
    }

    public string TimesOfDay 
    {
        get { return timesOfDay.Text; }
    }

    public string Duration 
    {
        get { return duration.Text; }
    }

    public string CallDate 
    {
        get { return callDate.Text; }
    }

    public string City 
    {
        get { return city.Text; }
    }

    public string Discount 
    {
        get { return discount.Text; }
    }

    public string PriceDaytime 
    {
        get { return priceDaytime.Text; }
    }

    public string PriceNight 
    {
        get { return priceNight.Text; }
    }
}