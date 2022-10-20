#include "pch.h"
#include "Payment.h"
#if __has_include("Payment.g.cpp")
#include "Payment.g.cpp"
#endif
#include <Globals.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Media::Imaging;
using namespace Controls;
using namespace Media::Animation;

using namespace std;
using namespace sqlite;

namespace winrt::HotelManagement::implementation
{
    int32_t Payment::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Payment::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


    void Payment::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {


        database db(dbPath());
        string user;

        db << "SELECT username FROM loggedin;"
            >> user;
     
        db << "SELECT paymentName FROM linked_eWallets WHERE username = ?;"
            << user
            >> [&](string paymentName) {
            if (paymentName == "GCash")
            {
                btnGcash().IsEnabled(false);
                btnGcash().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
            }
            else if (paymentName == "Maya")
            {
                btnPaymaya().IsEnabled(false);
                btnPaymaya().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
            }
            else if (paymentName == "Paypal")
            {
                btnPaypal().IsEnabled(false);
                btnPaypal().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
            }
        };

        db << "SELECT paymentName FROM linked_Cards WHERE username = ?;"
            << user
            >> [&](string paymentName) {
            if (paymentName == "BDO")
            {
                btnBDO().IsEnabled(false);
                btnBDO().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
            }
            else if (paymentName == "BPI")
            {
                btnBPI().IsEnabled(false);
                btnBPI().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
            }
            else if (paymentName == "Metrobank")
            {
                btnMetrobank().IsEnabled(false);
                btnMetrobank().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
            }
            else if (paymentName == "RCBC")
            {
                btnRCBC().IsEnabled(false);
                btnRCBC().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
            }
        };
    }
   
    Windows::Foundation::IAsyncAction Payment::btnGcash_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Link GCash Account");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/gcash.png"));

        auto img_icon = Image();
        img_icon.Source(bitmapImage);
        img_icon.Margin(ThicknessHelper::FromLengths(10, 20, 10, 20));
        img_icon.Height(90);
        img_icon.Width(200);


        auto subtitle = TextBlock();
        subtitle.Text(L"Enter your GCash account details below.");
        subtitle.TextAlignment(TextAlignment::Center);
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto account = TextBox();
        account.PlaceholderText(L"GCash Account / Number");
        account.Margin(marg);

        auto pin = PasswordBox();
        pin.PlaceholderText(L"GCash Password");
        pin.Margin(marg);

        auto acctPassword = PasswordBox();
        acctPassword.PlaceholderText(L"Confirm Account Password");
        acctPassword.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(img_icon);
        stack.Children().Append(subtitle);
        stack.Children().Append(account);
        stack.Children().Append(pin);
        stack.Children().Append(acctPassword);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Add Payment Method");

#pragma endregion

        auto res = co_await dialog.ShowAsync();


        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, password, paymentName = "GCash";
            int eWalletExists;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            db << "SELECT count(*) FROM linked_eWallets WHERE username = ? AND paymentName = ?;"
                << user
                << paymentName
                >> eWalletExists;

            if (eWalletExists) {
                co_await Dialog(
                    L"Payment Method Adding Error",
                    L"Payment method already linked to your account!",
                    L"Ok");
            }
            else
            {
                if (to_hstring(password) == acctPassword.Password()) {
                    hstring errors = L"";

                    if (account.Text().size() <= 6)
                        errors = errors + L"\n● Account / Number should be atleast 6 characters.";
                    else
                        if (to_string(account.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-") != string::npos)
                            errors = errors + L"\n● Account / Number should contains alphabets, numbers and specific characters (._-) only.";

                    if (acctPassword.Password().size() <= 6)
                        errors = errors + L"\n● Pin should be atleast 7 characters.";
                    else
                        if (to_string(acctPassword.Password()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-?#@*$&!") != string::npos)
                            errors = errors + L"\n● Pin should contains alphabets, numbers and special characters (._-?#@*$&!) only.";

                    if (errors.empty()) {
                         db << "INSERT INTO linked_eWallets ("
                                "   username     ,"
                                "   paymentName  ,"
                                "   acctName     ,"
                                "   pin          )"
                                "VALUES("
                                " '" + user + "', "
                                " '" + paymentName + "', "
                                " '" + to_string(account.Text()) + "', "
                                " '" + to_string(pin.Password()) + "'"
                                ");";

                         co_await Dialog(
                             L"Payment Method Added",
                             L"The eWallet account was successfully linked to your account!",
                             L"Ok");

                         btnGcash().IsEnabled(false);
                         btnGcash().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
                    }
                    else
                        co_await Dialog(
                            L"Input Error",
                            L"There are errors on your inputs:" + errors + L"\n\nPlease try again...",
                            L"Ok");
                }
                else
                {
                    co_await Dialog(
                        L"Account password error",
                        L"Confirmation paswword not matched to your account password.\nNo changes performed.",
                        L"Ok"
                    );
                }
            }
        }
    }


    Windows::Foundation::IAsyncAction Payment::btnPaymaya_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Link Maya Account");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/maya.png"));

        auto img_icon = Image();
        img_icon.Source(bitmapImage);
        img_icon.Margin(ThicknessHelper::FromLengths(10, 20, 10, 20));
        img_icon.Height(90);
        img_icon.Width(200);


        auto subtitle = TextBlock();
        subtitle.Text(L"Enter your Maya account details below.");
        subtitle.TextAlignment(TextAlignment::Center);
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto account = TextBox();
        account.PlaceholderText(L"Maya Account / Number");
        account.Margin(marg);

        auto pin = PasswordBox();
        pin.PlaceholderText(L"Maya Password");
        pin.Margin(marg);

        auto acctPassword = PasswordBox();
        acctPassword.PlaceholderText(L"Confirm Account Password");
        acctPassword.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(img_icon);
        stack.Children().Append(subtitle);
        stack.Children().Append(account);
        stack.Children().Append(pin);
        stack.Children().Append(acctPassword);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Add Payment Method");

#pragma endregion

        auto res = co_await dialog.ShowAsync();



        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, password, paymentName = "Maya";
            int eWalletExists;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            db << "SELECT count(*) FROM linked_eWallets WHERE username = ? AND paymentName = ?;"
                << user
                << paymentName
                >> eWalletExists;

            if (eWalletExists) {
                co_await Dialog(
                    L"Payment Method Adding Error",
                    L"Payment method already linked to your account!",
                    L"Ok");
            }
            else
            {
                if (to_hstring(password) == acctPassword.Password()) {
                    hstring errors = L"";

                    if (account.Text().size() <= 6)
                        errors = errors + L"\n● Account / Number should be atleast 6 characters.";
                    else
                        if (to_string(account.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-") != string::npos)
                            errors = errors + L"\n● Account / Number should contains alphabets, numbers and specific characters (._-) only.";

                    if (acctPassword.Password().size() <= 6)
                        errors = errors + L"\n● Pin should be atleast 7 characters.";
                    else
                        if (to_string(acctPassword.Password()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-?#@*$&!") != string::npos)
                            errors = errors + L"\n● Pin should contains alphabets, numbers and special characters (._-?#@*$&!) only.";

                    if (errors.empty()) {
                        db << "INSERT INTO linked_eWallets ("
                            "   username     ,"
                            "   paymentName  ,"
                            "   acctName     ,"
                            "   pin          )"
                            "VALUES("
                            " '" + user + "', "
                            " '" + paymentName + "', "
                            " '" + to_string(account.Text()) + "', "
                            " '" + to_string(pin.Password()) + "'"
                            ");";

                        co_await Dialog(
                            L"Payment Method Added",
                            L"The eWallet account was successfully linked to your account!",
                            L"Ok");

                        btnPaymaya().IsEnabled(false);
                        btnPaymaya().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
                    }
                    else
                        co_await Dialog(
                            L"Input Error",
                            L"There are errors on your inputs:" + errors + L"\n\nPlease try again...",
                            L"Ok");
                }
                else
                {
                    co_await Dialog(
                        L"Account password error",
                        L"Confirmation paswword not matched to your account password.\nNo changes performed.",
                        L"Ok"
                    );
                }
            }
        }
    }




    Windows::Foundation::IAsyncAction Payment::btnPaypal_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Link Paypal Account");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/paypal.png"));

        auto img_icon = Image();
        img_icon.Source(bitmapImage);
        img_icon.Margin(ThicknessHelper::FromLengths(10, 20, 10, 20));
        img_icon.Height(90);
        img_icon.Width(200);


        auto subtitle = TextBlock();
        subtitle.Text(L"Enter your Paypal account details below.");
        subtitle.TextAlignment(TextAlignment::Center);
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto account = TextBox();
        account.PlaceholderText(L"Paypal Account / Number");
        account.Margin(marg);

        auto pin = PasswordBox();
        pin.PlaceholderText(L"Paypal Password");
        pin.Margin(marg);

        auto acctPassword = PasswordBox();
        acctPassword.PlaceholderText(L"Confirm Account Password");
        acctPassword.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(img_icon);
        stack.Children().Append(subtitle);
        stack.Children().Append(account);
        stack.Children().Append(pin);
        stack.Children().Append(acctPassword);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Add Payment Method");

#pragma endregion

        auto res = co_await dialog.ShowAsync();


        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, password, paymentName = "Paypal";
            int eWalletExists;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            db << "SELECT count(*) FROM linked_eWallets WHERE username = ? AND paymentName = ?;"
                << user
                << paymentName
                >> eWalletExists;

            if (eWalletExists) {
                co_await Dialog(
                    L"Payment Method Adding Error",
                    L"Payment method already linked to your account!",
                    L"Ok");
            }
            else
            {
                if (to_hstring(password) == acctPassword.Password()) {
                    hstring errors = L"";

                    if (account.Text().size() <= 6)
                        errors = errors + L"\n● Account / Number should be atleast 6 characters.";
                    else
                        if (to_string(account.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-") != string::npos)
                            errors = errors + L"\n● Account / Number should contains alphabets, numbers and specific characters (._-) only.";

                    if (acctPassword.Password().size() <= 6)
                        errors = errors + L"\n● Pin should be atleast 7 characters.";
                    else
                        if (to_string(acctPassword.Password()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-?#@*$&!") != string::npos)
                            errors = errors + L"\n● Pin should contains alphabets, numbers and special characters (._-?#@*$&!) only.";

                    if (errors.empty()) {
                        db << "INSERT INTO linked_eWallets ("
                            "   username     ,"
                            "   paymentName  ,"
                            "   acctName     ,"
                            "   pin          )"
                            "VALUES("
                            " '" + user + "', "
                            " '" + paymentName + "', "
                            " '" + to_string(account.Text()) + "', "
                            " '" + to_string(pin.Password()) + "'"
                            ");";

                        co_await Dialog(
                            L"Payment Method Added",
                            L"The eWallet account was successfully linked to your account!",
                            L"Ok");

                        btnPaypal().IsEnabled(false);
                        btnPaypal().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
                    }
                    else
                        co_await Dialog(
                            L"Input Error",
                            L"There are errors on your inputs:" + errors + L"\n\nPlease try again...",
                            L"Ok");
                }
                else
                {
                    co_await Dialog(
                        L"Account password error",
                        L"Confirmation paswword not matched to your account password.\nNo changes performed.",
                        L"Ok"
                    );
                }
            }
        }
    }



    Windows::Foundation::IAsyncAction Payment::btnBDO_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Link BDO Card");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/bdo.png"));

        auto img_icon = Image();
        img_icon.Source(bitmapImage);
        img_icon.Margin(ThicknessHelper::FromLengths(10, 20, 10, 20));
        img_icon.Height(90);
        img_icon.Width(200);


        auto subtitle = TextBlock();
        subtitle.Text(L"Enter your BDO card details below.");
        subtitle.TextAlignment(TextAlignment::Center);
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto cardNum = TextBox();
        cardNum.PlaceholderText(L"Card Number (Format: 0000-0000-0000-0000)");
        cardNum.Margin(marg);


        auto grd_ = Grid();
        auto cldef1 = ColumnDefinition(); cldef1.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
        auto cldef2 = ColumnDefinition(); cldef2.Width(GridLengthHelper::FromPixels(120));
        grd_.ColumnDefinitions().Append(cldef1);
        grd_.ColumnDefinitions().Append(cldef2);

        marg.Right = 3;
        auto expiration = TextBox();
        expiration.PlaceholderText(L"Expiration (MM/YY)");
        expiration.Margin(marg);
        Grid::SetColumn(expiration, 0);

        marg.Left = 3;
        marg.Right = 0;
        auto cvv = TextBox();
        cvv.PlaceholderText(L"CVV");
        cvv.Margin(marg);
        Grid::SetColumn(cvv, 1);

        grd_.Children().Append(expiration);
        grd_.Children().Append(cvv);

        marg.Left = 0;
        marg.Right = 0;

        auto nameOnCard = TextBox();
        nameOnCard.PlaceholderText(L"Name On Card");
        nameOnCard.Margin(marg);

        auto confPass = PasswordBox();
        confPass.PlaceholderText(L"Confirm Account Password");
        confPass.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(img_icon);
        stack.Children().Append(subtitle);
        stack.Children().Append(cardNum);
        stack.Children().Append(grd_);
        stack.Children().Append(nameOnCard);
        stack.Children().Append(confPass);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Add Payment Method");

#pragma endregion

        auto res = co_await dialog.ShowAsync();



        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, password, paymentName = "BDO";
            int eWalletExists;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            db << "SELECT count(*) FROM linked_Cards WHERE username = ? AND paymentName = ?;"
                << user
                << paymentName
                >> eWalletExists;

            if (eWalletExists) {
                co_await Dialog(
                    L"Payment Method Adding Error",
                    L"Card already linked to your account!",
                    L"Ok");
            }
            else
            {
                if (to_hstring(password) == confPass.Password()) {
                    hstring errors = L"";

                    if (cardNum.Text().size() != 19)
                        errors = errors + L"\n● Card Number should be 19 characters only (16 numbers excluding dashes).";
                    else
                        if (to_string(cardNum.Text()).find_first_not_of("0123456789-") != string::npos)
                            errors = errors + L"\n● Card Number should contains numbers and 3 dashes (-) only.";

                    if (expiration.Text().size() != 5)
                        errors = errors + L"\n● Expiration should be 5 characters only.";
                    else
                        if (to_string(expiration.Text()).find_first_not_of("0123456789/") != string::npos)
                            errors = errors + L"\n● Expiration should contains numbers and 1 forward slash (/) only.";

                    if (cvv.Text().size() != 3)
                        errors = errors + L"\n● CVV should be 3 characters only.";
                    else
                        if (to_string(cvv.Text()).find_first_not_of("0123456789") != string::npos)
                            errors = errors + L"\n● CVV should contains numbers only.";

                    if (nameOnCard.Text().size() <= 7)
                        errors = errors + L"\n● Name on card should be atleast 7 characters only.";
                    else
                        if (to_string(nameOnCard.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz. ") != string::npos)
                            errors = errors + L"\n●  Name on card should contains alphabets and dot (.) only.";

                    if (errors.empty()) {
                        db << "INSERT INTO linked_Cards ("
                            "   username     ,"
                            "   paymentName  ,"
                            "   cardNumber   ,"
                            "   expiration   ,"
                            "   cvv          ,"
                            "   nameOnCard   )"
                            "VALUES("
                            " '" + user + "', "
                            " '" + paymentName + "', "
                            " '" + to_string(cardNum.Text()) + "', "
                            " '" + to_string(expiration.Text()) + "', "
                            " '" + to_string(cvv.Text()) + "', "
                            " '" + to_string(nameOnCard.Text()) + "'"
                            ");";

                        co_await Dialog(
                            L"Payment Method Added",
                            L"The card was successfully linked to your account!",
                            L"Ok");

                        btnBDO().IsEnabled(false);
                        btnBDO().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
                    }
                    else
                        co_await Dialog(
                            L"Input Error",
                            L"There are errors on your inputs:" + errors + L"\n\nPlease try again...",
                            L"Ok");
                }
                else
                {
                    co_await Dialog(
                        L"Account password error",
                        L"Confirmation paswword not matched to your account password.\nNo changes performed.",
                        L"Ok"
                    );
                }
            }
        }
    }



    Windows::Foundation::IAsyncAction Payment::btnBPI_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Link BPI Card");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/bpi.png"));

        auto img_icon = Image();
        img_icon.Source(bitmapImage);
        img_icon.Margin(ThicknessHelper::FromLengths(10, 20, 10, 20));
        img_icon.Height(90);
        img_icon.Width(200);


        auto subtitle = TextBlock();
        subtitle.Text(L"Enter your BPI card details below.");
        subtitle.TextAlignment(TextAlignment::Center);
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto cardNum = TextBox();
        cardNum.PlaceholderText(L"Card Number");
        cardNum.Margin(marg);


        auto grd_ = Grid();
        auto cldef1 = ColumnDefinition(); cldef1.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
        auto cldef2 = ColumnDefinition(); cldef2.Width(GridLengthHelper::FromPixels(120));
        grd_.ColumnDefinitions().Append(cldef1);
        grd_.ColumnDefinitions().Append(cldef2);

        marg.Right = 3;
        auto expiration = TextBox();
        expiration.PlaceholderText(L"Expiration (MM/YY)");
        expiration.Margin(marg);
        Grid::SetColumn(expiration, 0);

        marg.Left = 3;
        marg.Right = 0;
        auto cvv = TextBox();
        cvv.PlaceholderText(L"CVV");
        cvv.Margin(marg);
        Grid::SetColumn(cvv, 1);

        grd_.Children().Append(expiration);
        grd_.Children().Append(cvv);

        marg.Left = 0;
        marg.Right = 0;

        auto nameOnCard = TextBox();
        nameOnCard.PlaceholderText(L"Name On Card");
        nameOnCard.Margin(marg);

        auto confPass = PasswordBox();
        confPass.PlaceholderText(L"Confirm Account Password");
        confPass.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(img_icon);
        stack.Children().Append(subtitle);
        stack.Children().Append(cardNum);
        stack.Children().Append(grd_);
        stack.Children().Append(nameOnCard);
        stack.Children().Append(confPass);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Add Payment Method");

#pragma endregion

        auto res = co_await dialog.ShowAsync();


        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, password, paymentName = "BPI";
            int eWalletExists;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            db << "SELECT count(*) FROM linked_Cards WHERE username = ? AND paymentName = ?;"
                << user
                << paymentName
                >> eWalletExists;

            if (eWalletExists) {
                co_await Dialog(
                    L"Payment Method Adding Error",
                    L"Card already linked to your account!",
                    L"Ok");
            }
            else
            {
                if (to_hstring(password) == confPass.Password()) {
                    hstring errors = L"";

                    if (cardNum.Text().size() != 19)
                        errors = errors + L"\n● Card Number should be 19 characters only (16 numbers excluding dashes).";
                    else
                        if (to_string(cardNum.Text()).find_first_not_of("0123456789-") != string::npos)
                            errors = errors + L"\n● Card Number should contains numbers and 3 dashes (-) only.";

                    if (expiration.Text().size() != 5)
                        errors = errors + L"\n● Expiration should be 5 characters only.";
                    else
                        if (to_string(expiration.Text()).find_first_not_of("0123456789/") != string::npos)
                            errors = errors + L"\n● Expiration should contains numbers and 1 forward slash (/) only.";

                    if (cvv.Text().size() != 3)
                        errors = errors + L"\n● CVV should be 3 characters only.";
                    else
                        if (to_string(cvv.Text()).find_first_not_of("0123456789") != string::npos)
                            errors = errors + L"\n● CVV should contains numbers only.";

                    if (nameOnCard.Text().size() <= 7)
                        errors = errors + L"\n● Name on card should be atleast 7 characters only.";
                    else
                        if (to_string(nameOnCard.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz. ") != string::npos)
                            errors = errors + L"\n●  Name on card should contains alphabets and dot (.) only.";

                    if (errors.empty()) {
                        db << "INSERT INTO linked_Cards ("
                            "   username     ,"
                            "   paymentName  ,"
                            "   cardNumber   ,"
                            "   expiration   ,"
                            "   cvv          ,"
                            "   nameOnCard   )"
                            "VALUES("
                            " '" + user + "', "
                            " '" + paymentName + "', "
                            " '" + to_string(cardNum.Text()) + "', "
                            " '" + to_string(expiration.Text()) + "', "
                            " '" + to_string(cvv.Text()) + "', "
                            " '" + to_string(nameOnCard.Text()) + "'"
                            ");";

                        co_await Dialog(
                            L"Payment Method Added",
                            L"The card was successfully linked to your account!",
                            L"Ok");

                        btnBPI().IsEnabled(false);
                        btnBPI().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
                    }
                    else
                        co_await Dialog(
                            L"Input Error",
                            L"There are errors on your inputs:" + errors + L"\n\nPlease try again...",
                            L"Ok");
                }
                else
                {
                    co_await Dialog(
                        L"Account password error",
                        L"Confirmation paswword not matched to your account password.\nNo changes performed.",
                        L"Ok"
                    );
                }
            }
        }
    }



    Windows::Foundation::IAsyncAction Payment::btnMetrobank_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Link Metrobank Card");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/metrobank.png"));

        auto img_icon = Image();
        img_icon.Source(bitmapImage);
        img_icon.Margin(ThicknessHelper::FromLengths(10, 20, 10, 20));
        img_icon.Height(90);
        img_icon.Width(200);


        auto subtitle = TextBlock();
        subtitle.Text(L"Enter your Metrobank card details below.");
        subtitle.TextAlignment(TextAlignment::Center);
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto cardNum = TextBox();
        cardNum.PlaceholderText(L"Card Number");
        cardNum.Margin(marg);


        auto grd_ = Grid();
        auto cldef1 = ColumnDefinition(); cldef1.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
        auto cldef2 = ColumnDefinition(); cldef2.Width(GridLengthHelper::FromPixels(120));
        grd_.ColumnDefinitions().Append(cldef1);
        grd_.ColumnDefinitions().Append(cldef2);

        marg.Right = 3;
        auto expiration = TextBox();
        expiration.PlaceholderText(L"Expiration (MM/YY)");
        expiration.Margin(marg);
        Grid::SetColumn(expiration, 0);

        marg.Left = 3;
        marg.Right = 0;
        auto cvv = TextBox();
        cvv.PlaceholderText(L"CVV");
        cvv.Margin(marg);
        Grid::SetColumn(cvv, 1);

        grd_.Children().Append(expiration);
        grd_.Children().Append(cvv);

        marg.Left = 0;
        marg.Right = 0;

        auto nameOnCard = TextBox();
        nameOnCard.PlaceholderText(L"Name On Card");
        nameOnCard.Margin(marg);

        auto confPass = PasswordBox();
        confPass.PlaceholderText(L"Confirm Account Password");
        confPass.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(img_icon);
        stack.Children().Append(subtitle);
        stack.Children().Append(cardNum);
        stack.Children().Append(grd_);
        stack.Children().Append(nameOnCard);
        stack.Children().Append(confPass);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Add Payment Method");

#pragma endregion

        auto res = co_await dialog.ShowAsync();


        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, password, paymentName = "Metrobank";
            int eWalletExists;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            db << "SELECT count(*) FROM linked_Cards WHERE username = ? AND paymentName = ?;"
                << user
                << paymentName
                >> eWalletExists;

            if (eWalletExists) {
                co_await Dialog(
                    L"Payment Method Adding Error",
                    L"Card already linked to your account!",
                    L"Ok");
            }
            else
            {
                if (to_hstring(password) == confPass.Password()) {
                    hstring errors = L"";

                    if (cardNum.Text().size() != 19)
                        errors = errors + L"\n● Card Number should be 19 characters only (16 numbers excluding dashes).";
                    else
                        if (to_string(cardNum.Text()).find_first_not_of("0123456789-") != string::npos)
                            errors = errors + L"\n● Card Number should contains numbers and 3 dashes (-) only.";

                    if (expiration.Text().size() != 5)
                        errors = errors + L"\n● Expiration should be 5 characters only.";
                    else
                        if (to_string(expiration.Text()).find_first_not_of("0123456789/") != string::npos)
                            errors = errors + L"\n● Expiration should contains numbers and 1 forward slash (/) only.";

                    if (cvv.Text().size() != 3)
                        errors = errors + L"\n● CVV should be 3 characters only.";
                    else
                        if (to_string(cvv.Text()).find_first_not_of("0123456789") != string::npos)
                            errors = errors + L"\n● CVV should contains numbers only.";

                    if (nameOnCard.Text().size() <= 7)
                        errors = errors + L"\n● Name on card should be atleast 7 characters only.";
                    else
                        if (to_string(nameOnCard.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz. ") != string::npos)
                            errors = errors + L"\n●  Name on card should contains alphabets and dot (.) only.";

                    if (errors.empty()) {
                        db << "INSERT INTO linked_Cards ("
                            "   username     ,"
                            "   paymentName  ,"
                            "   cardNumber   ,"
                            "   expiration   ,"
                            "   cvv          ,"
                            "   nameOnCard   )"
                            "VALUES("
                            " '" + user + "', "
                            " '" + paymentName + "', "
                            " '" + to_string(cardNum.Text()) + "', "
                            " '" + to_string(expiration.Text()) + "', "
                            " '" + to_string(cvv.Text()) + "', "
                            " '" + to_string(nameOnCard.Text()) + "'"
                            ");";

                        co_await Dialog(
                            L"Payment Method Added",
                            L"The card was successfully linked to your account!",
                            L"Ok");

                        btnMetrobank().IsEnabled(false);
                        btnMetrobank().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
                    }
                    else
                        co_await Dialog(
                            L"Input Error",
                            L"There are errors on your inputs:" + errors + L"\n\nPlease try again...",
                            L"Ok");
                }
                else
                {
                    co_await Dialog(
                        L"Account password error",
                        L"Confirmation paswword not matched to your account password.\nNo changes performed.",
                        L"Ok"
                    );
                }
            }
        }
    }



    Windows::Foundation::IAsyncAction Payment::btnRCBC_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
#pragma region "Generate Dialog"

        auto title = TextBlock();
        title.Text(L"Link RCBC Card");

        auto marg = Thickness();
        marg.Bottom = 15; marg.Top = 5;

        auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/rcbc.png"));

        auto img_icon = Image();
        img_icon.Source(bitmapImage);
        img_icon.Margin(ThicknessHelper::FromLengths(10, 20, 10, 20));
        img_icon.Height(90);
        img_icon.Width(200);


        auto subtitle = TextBlock();
        subtitle.Text(L"Enter your RCBC card details below.");
        subtitle.TextAlignment(TextAlignment::Center);
        subtitle.Width(400);
        subtitle.TextWrapping(TextWrapping::WrapWholeWords);
        subtitle.Margin(marg);

        marg.Bottom = 5;

        auto cardNum = TextBox();
        cardNum.PlaceholderText(L"Card Number");
        cardNum.Margin(marg);


        auto grd_ = Grid();
        auto cldef1 = ColumnDefinition(); cldef1.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
        auto cldef2 = ColumnDefinition(); cldef2.Width(GridLengthHelper::FromPixels(120));
        grd_.ColumnDefinitions().Append(cldef1);
        grd_.ColumnDefinitions().Append(cldef2);

        marg.Right = 3;
        auto expiration = TextBox();
        expiration.PlaceholderText(L"Expiration (MM/YY)");
        expiration.Margin(marg);
        Grid::SetColumn(expiration, 0);

        marg.Left = 3;
        marg.Right = 0;
        auto cvv = TextBox();
        cvv.PlaceholderText(L"CVV");
        cvv.Margin(marg);
        Grid::SetColumn(cvv, 1);

        grd_.Children().Append(expiration);
        grd_.Children().Append(cvv);

        marg.Left = 0;
        marg.Right = 0;

        auto nameOnCard = TextBox();
        nameOnCard.PlaceholderText(L"Name On Card");
        nameOnCard.Margin(marg);

        auto confPass = PasswordBox();
        confPass.PlaceholderText(L"Confirm Account Password");
        confPass.Margin(marg);

        auto stack = StackPanel();
        stack.Children().Append(img_icon);
        stack.Children().Append(subtitle);
        stack.Children().Append(cardNum);
        stack.Children().Append(grd_);
        stack.Children().Append(nameOnCard);
        stack.Children().Append(confPass);


        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);

        dialog.Content(stack);
        dialog.CloseButtonText(L"Cancel");
        dialog.PrimaryButtonText(L"Add Payment Method");

#pragma endregion

        auto res = co_await dialog.ShowAsync();


        database db(dbPath());
        if (res == ContentDialogResult::Primary) {

            string user, password, paymentName = "RCBC";
            int eWalletExists;

            db << "SELECT username, password FROM loggedin;"
                >> tie(user, password);

            db << "SELECT count(*) FROM linked_Cards WHERE username = ? AND paymentName = ?;"
                << user
                << paymentName
                >> eWalletExists;

            if (eWalletExists) {
                co_await Dialog(
                    L"Payment Method Adding Error",
                    L"Card already linked to your account!",
                    L"Ok");
            }
            else
            {
                if (to_hstring(password) == confPass.Password()) {
                    hstring errors = L"";

                    if (cardNum.Text().size() != 19)
                        errors = errors + L"\n● Card Number should be 19 characters only (16 numbers excluding dashes).";
                    else
                        if (to_string(cardNum.Text()).find_first_not_of("0123456789-") != string::npos)
                            errors = errors + L"\n● Card Number should contains numbers and 3 dashes (-) only.";

                    if (expiration.Text().size() != 5)
                        errors = errors + L"\n● Expiration should be 5 characters only.";
                    else
                        if (to_string(expiration.Text()).find_first_not_of("0123456789/") != string::npos)
                            errors = errors + L"\n● Expiration should contains numbers and 1 forward slash (/) only.";

                    if (cvv.Text().size() != 3)
                        errors = errors + L"\n● CVV should be 3 characters only.";
                    else
                        if (to_string(cvv.Text()).find_first_not_of("0123456789") != string::npos)
                            errors = errors + L"\n● CVV should contains numbers only.";

                    if (nameOnCard.Text().size() <= 7)
                        errors = errors + L"\n● Name on card should be atleast 7 characters only.";
                    else
                        if (to_string(nameOnCard.Text()).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz. ") != string::npos)
                            errors = errors + L"\n●  Name on card should contains alphabets and dot (.) only.";

                    if (errors.empty()) {
                        db << "INSERT INTO linked_Cards ("
                            "   username     ,"
                            "   paymentName  ,"
                            "   cardNumber   ,"
                            "   expiration   ,"
                            "   cvv          ,"
                            "   nameOnCard   )"
                            "VALUES("
                            " '" + user + "', "
                            " '" + paymentName + "', "
                            " '" + to_string(cardNum.Text()) + "', "
                            " '" + to_string(expiration.Text()) + "', "
                            " '" + to_string(cvv.Text()) + "', "
                            " '" + to_string(nameOnCard.Text()) + "'"
                            ");";

                        co_await Dialog(
                            L"Payment Method Added",
                            L"The card was successfully linked to your account!",
                            L"Ok");

                        btnRCBC().IsEnabled(false); 
                        btnRCBC().Content().as<Grid>().Children().GetAt(0).as<Image>().Opacity(0.5);
                    }
                    else
                        co_await Dialog(
                            L"Input Error",
                            L"There are errors on your inputs:" + errors + L"\n\nPlease try again...",
                            L"Ok");
                }
                else
                {
                    co_await Dialog(
                        L"Account password error",
                        L"Confirmation paswword not matched to your account password.\nNo changes performed.",
                        L"Ok"
                    );
                }
            }
        }
    }

    void Payment::HomeTitle_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto t = SlideNavigationTransitionInfo();
        t.Effect(SlideNavigationTransitionEffect::FromLeft);

        Frame().Navigate(winrt::xaml_typename<Account>(), nullptr, t);
    }

}




