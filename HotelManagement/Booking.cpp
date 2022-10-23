#include "pch.h"
#include "Booking.h"
#if __has_include("Booking.g.cpp")
#include "Booking.g.cpp"
#endif

#include "Globals.h"
#include <winrt/Windows.Globalization.DateTimeFormatting.h>
#include <winrt/Windows.System.Threading.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>

using namespace winrt::Microsoft::UI::Xaml::Controls;

using namespace winrt;
using namespace Windows;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Animation;
using namespace Imaging;
using namespace Controls;
using namespace Controls::Primitives;
using namespace Windows::Foundation;

using namespace std;
using namespace sqlite;

namespace winrt::HotelManagement::implementation
{
	int32_t Booking::MyProperty()
	{
		throw hresult_not_implemented();
	}

	void Booking::MyProperty(int32_t /* value */)
	{
		throw hresult_not_implemented();
	}



	void Booking::tgbtn_junior_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		insertOptions(sender.as<ToggleButton>(), 990);
	}


	void Booking::tgbtn_junior_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		auto childs = sender.as<ToggleButton>().Parent().as<StackPanel>();
		childs.Children().RemoveAt(1);

		auto childs_st = childs.Children().GetAt(1).as<StackPanel>();
		childs_st.Children().Clear();

		all_values.erase(all_values.find("Junior Suite"));
	}


	void Booking::tgbtn_superior_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		insertOptions(sender.as<ToggleButton>(), 1590);

	}


	void Booking::tgbtn_superior_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		auto childs = sender.as<ToggleButton>().Parent().as<StackPanel>();
		childs.Children().RemoveAt(1);

		auto childs_st = childs.Children().GetAt(1).as<StackPanel>();
		childs_st.Children().Clear();

		all_values.erase(all_values.find("Superiror Deluxe Room"));
	}


	void Booking::tgbtn_classic_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		insertOptions(sender.as<ToggleButton>(), 790);

	}


	void Booking::tgbtn_classic_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		auto childs = sender.as<ToggleButton>().Parent().as<StackPanel>();
		childs.Children().RemoveAt(1);

		auto childs_st = childs.Children().GetAt(1).as<StackPanel>();
		childs_st.Children().Clear();

		all_values.erase(all_values.find("Classic Deluxe Room"));

	}


	void Booking::tgbtn_honeymoon_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		insertOptions(sender.as<ToggleButton>(), 1690);

	}


	void Booking::tgbtn_honeymoon_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		auto childs = sender.as<ToggleButton>().Parent().as<StackPanel>();
		childs.Children().RemoveAt(1);

		auto childs_st = childs.Children().GetAt(1).as<StackPanel>();
		childs_st.Children().Clear();

		all_values.erase(all_values.find("Honeymoon Suite"));

	}


	void Booking::tgbtn_executive_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		insertOptions(sender.as<ToggleButton>(), 1990);

	}


	void Booking::tgbtn_executive_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		auto childs = sender.as<ToggleButton>().Parent().as<StackPanel>();
		childs.Children().RemoveAt(1);

		auto childs_st = childs.Children().GetAt(1).as<StackPanel>();
		childs_st.Children().Clear();

		all_values.erase(all_values.find("Executive Suite"));

	}


	void Booking::updateBooking() {

		database db(dbPath());
		string user;

		lst_book().Items().Clear();

		db << "SELECT username FROM loggedin;"
			>> user;

		int book_count;
		double total = 0;
		db << "SELECT COUNT(*) FROM booking_room WHERE username = ?;"
			<< user
			>> book_count;

		map <string, int> prices;
		prices.insert_or_assign("Classic Deluxe Room", 790);
		prices.insert_or_assign("Superiror Deluxe Room", 1590);
		prices.insert_or_assign("Junior Suite", 990);
		prices.insert_or_assign("Honeymoon Suite", 1690);
		prices.insert_or_assign("Executive Suite", 1990);

		if (book_count != 0) {
			brd_book().Visibility(Visibility::Visible);
			db << "SELECT * FROM booking_room WHERE username = ?;"
				<< user
				>> [&](
					int id,
					string username,
					string room,
					int roon_num,
					int days,
					string check_in,
					int payed
					) {
						total += days * prices[room];

						auto bd_itm = Border();
						bd_itm.CornerRadius(CornerRadiusHelper::FromUniformRadius(3));
						bd_itm.Padding(ThicknessHelper::FromUniformLength(20));
						bd_itm.Width(290);
						bd_itm.Height(105);

						auto grd_itm = Grid();


						auto itm = StackPanel();

						auto room_ob = TextBlock();
						room_ob.FontSize(17);
						room_ob.Margin(ThicknessHelper::FromLengths(0, 0, 0, 5));
						room_ob.HorizontalAlignment(HorizontalAlignment::Left);
						room_ob.Text(to_hstring(room));

						itm.Children().Append(room_ob);



						auto checkIn_ob = TextBlock();
						checkIn_ob.FontFamily(winrt::unbox_value<Windows::UI::Xaml::Media::FontFamily>(
							Application::Current().Resources().Lookup(winrt::box_value(L"Light"))));
						checkIn_ob.FontSize(14);
						checkIn_ob.HorizontalAlignment(HorizontalAlignment::Left);
						checkIn_ob.Text(L"Check-in date: " + to_hstring(check_in));

						itm.Children().Append(checkIn_ob);



						auto days_ob = TextBlock();
						days_ob.FontFamily(winrt::unbox_value<Windows::UI::Xaml::Media::FontFamily>(
							Application::Current().Resources().Lookup(winrt::box_value(L"Light"))));
						days_ob.FontSize(14);
						days_ob.HorizontalAlignment(HorizontalAlignment::Left);
						days_ob.Text(to_hstring(roon_num) + L" day check-in");

						itm.Children().Append(days_ob);

						grd_itm.Children().Append(itm);

						if (payed == 1) {
							auto brd_pyd = Border();
							brd_pyd.CornerRadius(CornerRadiusHelper::FromUniformRadius(4));
							brd_pyd.Padding(ThicknessHelper::FromLengths(10, 5, 10, 5));
							brd_pyd.HorizontalAlignment(HorizontalAlignment::Right);
							brd_pyd.VerticalAlignment(VerticalAlignment::Top);


							auto ol_pyd = SolidColorBrush();
							ol_pyd.Color(winrt::unbox_value<Windows::UI::Color>(
								Application::Current().Resources().Lookup(winrt::box_value(L"SystemAccentColor")
								)));

							ol_pyd.Opacity(0.75);

							brd_pyd.Background(ol_pyd);


							auto txt_pyd = TextBlock();
							txt_pyd.Text(L"Payed");


							brd_pyd.Child(txt_pyd);
							grd_itm.Children().Append(brd_pyd);

						}

						bd_itm.Tag(box_value(to_hstring(id)));
						bd_itm.Child(grd_itm);

						auto ls = lst_book();




						lst_book().Items().Append(bd_itm);

			};
		}
		else
			brd_book().Visibility(Visibility::Collapsed);

		txt_overall().Text(L"Overall Price: ₱" + to_hstring(total));
	}



	void Booking::Page_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		updateBooking();



	}

	void Booking::insertOptions(DependencyObject const obj, int price)
	{

		auto grd_obj = obj.as<ToggleButton>().Parent().as<StackPanel>();

		auto control = Grid();
		auto cont_cldef1 = ColumnDefinition(); cont_cldef1.Width(GridLengthHelper::FromPixels(90));
		auto cont_cldef3 = ColumnDefinition(); cont_cldef3.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
		auto cont_cldef4 = ColumnDefinition(); cont_cldef4.Width(GridLengthHelper::FromPixels(160));
		control.ColumnDefinitions().Append(cont_cldef1);
		control.ColumnDefinitions().Append(cont_cldef3);
		control.ColumnDefinitions().Append(cont_cldef4);
		auto cont_rwdef1 = RowDefinition(); cont_rwdef1.Height(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
		auto cont_rwdef2 = RowDefinition(); cont_rwdef2.Height(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
		control.RowDefinitions().Append(cont_rwdef1);
		control.RowDefinitions().Append(cont_rwdef2);

		if (grd_obj.Children().Size() == 2) {
			auto add = StackPanel();
			add.Margin(ThicknessHelper::FromUniformLength(10));
			add.HorizontalAlignment(HorizontalAlignment::Center);
			add.Orientation(Orientation::Horizontal);

			auto btn_add = Button();
			btn_add.Content(box_value(L"Add room"));
			btn_add.Tag(box_value(to_hstring(price)));
			add.Children().Append(btn_add);

			auto txt_rmCnt = TextBlock();
			txt_rmCnt.Margin(ThicknessHelper::FromLengths(20, 0, 0, 0));
			txt_rmCnt.TextAlignment(TextAlignment::Center);
			txt_rmCnt.VerticalAlignment(VerticalAlignment::Center);
			txt_rmCnt.FontSize(15);
			txt_rmCnt.Text(L"Room count: 1");
			add.Children().Append(txt_rmCnt);


			auto nm = to_string(obj.as<ToggleButton>().Content().as<Grid>().Children().GetAt(0).as<TextBlock>().Text());
			vector<array<string, 2>> values;
			all_values.insert_or_assign(nm, values);

			btn_add.Click([&](IInspectable const& sender_c, RoutedEventArgs const& e_c) {
				auto stc = sender_c.as<Button>().Parent().as<StackPanel>();
				auto stc1 = stc.Parent().as<StackPanel>();
				insertOptions(stc1.Children().GetAt(0).as<ToggleButton>(), std::stoi(unbox_value<hstring>(sender_c.as<Button>().Tag()).c_str()));

				auto stc2 = stc1.Children().GetAt(2).as<StackPanel>();
				stc.Children().GetAt(1).as<TextBlock>().Text(L"Room count: " + to_hstring(stc2.Children().Size()));
				});
			grd_obj.Children().InsertAt(1, add);
		}




		auto bg_cont = Border();
		bg_cont.Margin(ThicknessHelper::FromUniformLength(5));
		bg_cont.CornerRadius(CornerRadiusHelper::FromUniformRadius(5));
		bg_cont.Background(winrt::unbox_value<Windows::UI::Xaml::Media::Brush>(
			Application::Current().Resources().Lookup(winrt::box_value(L"ButtonBackground")
			)));
		Grid::SetRowSpan(bg_cont, 2);
		Grid::SetColumnSpan(bg_cont, 3);



		auto bg_rm = Border();
		bg_rm.Margin(ThicknessHelper::FromLengths(13, 13, 0, 13));
		bg_rm.Padding(ThicknessHelper::FromLengths(5, 10, 5, 10));
		bg_rm.CornerRadius(CornerRadiusHelper::FromUniformRadius(5));
		bg_rm.Background(winrt::unbox_value<Windows::UI::Xaml::Media::Brush>(
			Application::Current().Resources().Lookup(winrt::box_value(L"ButtonBackground")
			)));
		Grid::SetRow(bg_rm, 0);
		Grid::SetColumn(bg_rm, 0);

		auto txt_pg = TextBlock();
		txt_pg.TextAlignment(TextAlignment::Center);
		txt_pg.FontSize(13);

		bg_rm.Child(txt_pg);


		auto grd_day = Grid();
		auto day_cldef1 = ColumnDefinition(); day_cldef1.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
		auto day_cldef2 = ColumnDefinition(); day_cldef2.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
		grd_day.Margin(ThicknessHelper::FromLengths(13, 0, 13, 13));
		grd_day.ColumnDefinitions().Append(day_cldef1);
		grd_day.ColumnDefinitions().Append(day_cldef2);
		grd_day.MaxWidth(500);
		grd_day.MinWidth(250);



		auto date = CalendarDatePicker();
		date.HorizontalAlignment(HorizontalAlignment::Stretch);
		date.Margin(ThicknessHelper::FromLengths(0, 0, 10, 0));
		date.PlaceholderText(L"Select check-in date");
		date.DateChanged([&](CalendarDatePicker const& sender_c, CalendarDatePickerDateChangedEventArgs const& args_c) {
			auto outer = sender_c.Parent().as<Grid>();
			auto outer_1 = outer.Parent().as<Grid>().Parent().as<StackPanel>().Parent().as<StackPanel>();

			auto txt_name = to_string(outer_1.Children().GetAt(0).as<ToggleButton>().Content().as<Grid>().Children().GetAt(0).as<TextBlock>().Text());

			auto fmt_date = Globalization::DateTimeFormatting::DateTimeFormatter(L"{year.full}/{month.integer(2)}/{day.integer(2)}");
			auto fmttd_date = to_string(fmt_date.Format(args_c.NewDate().Value()));
			auto room_val = outer.Children().GetAt(1).as<NumberBox>();
			auto room_value = std::to_string((int)room_val.Value());

			auto indx = std::stoi(to_string(unbox_value<hstring>(outer.Tag())));

			if (room_val.Text() != L"") {
				all_values[txt_name][indx] = { room_value, fmttd_date };
			}
			else {
				all_values[txt_name][indx] = { "", fmttd_date };
			}


			});

		//source: https://stackoverflow.com/questions/43010362/c-add-months-to-chronosystem-clocktime-point
		using days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
		using years = std::chrono::duration<int, std::ratio_multiply<std::ratio<146097, 400>, days::period>>;

		date.MaxDate(winrt::clock::now() + years(3));
		date.MinDate(winrt::clock::now() + days(3));



		Grid::SetColumn(date, 0);
		grd_day.Children().Append(date);


		auto num_day = NumberBox();
		num_day.VerticalAlignment(VerticalAlignment::Center);
		num_day.Maximum(60);
		num_day.Minimum(1);
		num_day.SpinButtonPlacementMode(NumberBoxSpinButtonPlacementMode::Inline);
		num_day.PlaceholderText(L"Select check-in days");
		num_day.Tag(box_value(to_hstring(price)));
		num_day.ValueChanged([&](NumberBox const& sender_v, NumberBoxValueChangedEventArgs const& args_v) {
			auto outer = sender_v.Parent().as<Grid>();
			auto outer_1 = outer.Parent().as<Grid>().Parent().as<StackPanel>().Parent().as<StackPanel>();


			auto fmt_date = Globalization::DateTimeFormatting::DateTimeFormatter(L"{year.full}/{month.integer(2)}/{day.integer(2)}");

			auto txt_name = to_string(outer_1.Children().GetAt(0).as<ToggleButton>().Content().as<Grid>().Children().GetAt(0).as<TextBlock>().Text());

			auto calendar_val = outer.Children().GetAt(0).as<CalendarDatePicker>().Date();
			auto indx = std::stoi(to_string(unbox_value<hstring>(outer.Tag())));

			if (calendar_val != nullptr)
			{
				auto calendar_value = to_string(fmt_date.Format(calendar_val.Value()));
				all_values[txt_name][indx] = { std::to_string((int)args_v.NewValue()), calendar_value };
				outer.Parent().as<Grid>().Children().GetAt(4).as<Border>().Child().as<TextBlock>().Text(L"₱" + to_hstring(std::stoi(unbox_value<hstring>(sender_v.Tag()).c_str()) * args_v.NewValue()));
			}
			else {
				all_values[txt_name][indx] = { std::to_string((int)args_v.NewValue()), "" };
				outer.Parent().as<Grid>().Children().GetAt(4).as<Border>().Child().as<TextBlock>().Text(L"");

			}



			});
		Grid::SetColumn(num_day, 1);

		grd_day.Children().Append(num_day);

		Grid::SetColumnSpan(grd_day, 3);
		Grid::SetRow(grd_day, 1);



		auto bg_ttl = Border();
		bg_ttl.Margin(ThicknessHelper::FromLengths(0, 13, 13, 13));
		bg_ttl.Padding(ThicknessHelper::FromLengths(13, 5, 13, 5));
		bg_ttl.CornerRadius(CornerRadiusHelper::FromUniformRadius(5));
		bg_ttl.Background(unbox_value<Brush>(
			Application::Current().Resources().Lookup(winrt::box_value(L"ButtonBackground")
			)));
		Grid::SetRow(bg_ttl, 0);
		Grid::SetColumn(bg_ttl, 3);

		auto txt_ttl = TextBlock();
		txt_ttl.VerticalAlignment(VerticalAlignment::Center);
		txt_ttl.FontSize(13);
		txt_ttl.Text(L"Total");

		bg_ttl.Child(txt_ttl);



		auto bg_ttlVal = Border();
		bg_ttlVal.Margin(ThicknessHelper::FromLengths(55, 13, 13, 13));
		bg_ttlVal.Padding(ThicknessHelper::FromLengths(13, 5, 13, 5));
		bg_ttlVal.CornerRadius(CornerRadiusHelper::FromUniformRadius(5));
		bg_ttlVal.Background(unbox_value<Brush>(
			Application::Current().Resources().Lookup(winrt::box_value(L"ButtonBackground")
			)));
		Grid::SetRow(bg_ttlVal, 0);
		Grid::SetColumn(bg_ttlVal, 3);

		auto txt_ttlVal = TextBlock();
		txt_ttlVal.TextAlignment(TextAlignment::Center);
		txt_ttlVal.VerticalAlignment(VerticalAlignment::Center);
		txt_ttlVal.FontSize(13);
		txt_ttlVal.Text(L"₱" + to_hstring(price));

		bg_ttlVal.Child(txt_ttlVal);



		auto nm = to_string(obj.as<ToggleButton>().Content().as<Grid>().Children().GetAt(0).as<TextBlock>().Text());
		all_values[nm].push_back({ "", "" });


		control.Children().Append(bg_cont);
		control.Children().Append(bg_rm);
		control.Children().Append(grd_day);
		control.Children().Append(bg_ttl);
		control.Children().Append(bg_ttlVal);



		auto grd_obj_main = grd_obj.Children().GetAt(2).as<StackPanel>();
		txt_pg.Text(L"Room " + to_hstring(grd_obj_main.Children().Size() + 1));
		grd_day.Tag(box_value(to_hstring(grd_obj_main.Children().Size())));



		grd_obj_main.Children().Append(control);

	}


	void Booking::btnConfirm_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{
		database db(dbPath());
		string user;
		//string res = ""; 

		db << "SELECT username FROM loggedin;"
			>> user;

		if (all_values.size() == 0) {
			Dialog(
				L"Error creating a booking",
				L"Not selected any units/rooms.",
				L"Ok"
			);
			return;
		}

		for (auto mapIt = begin(all_values); mapIt != end(all_values); ++mapIt)
		{
			//res += mapIt->first + " :\n";

			for (auto c : mapIt->second)
			{
				if (c[0].empty() || c[1].empty())
				{
					Dialog(
						L"Error creating a booking",
						L"Check the check-in days and check-in dates, some are not filled up well. Please try again.",
						L"Ok"
					);
					return;
				}
				//res +=  c[0] + " | " + c[1] + "\n";
			}

			//res += "\n\n";
		}

		/* Dialog(
			 L"VECTOR",
			 to_hstring(res),
			 L"Exit"
		 );*/

		Dialog(
			L"Booking added",
			L"The booking created successfully.",
			L"Ok"
		);

		for (auto mapIt = begin(all_values); mapIt != end(all_values); ++mapIt)
		{
			int i = 0;
			for (auto c : mapIt->second)
			{
				i++;
				db << "INSERT INTO booking_room ("
					"  username   ,"
					"  room       ,"
					"  room_num   ,"
					"  days       ,"
					"  check_in   ,"
					"  payed    "
					")"
					"VALUES("
					" '" + user + "', "
					" '" + mapIt->first + "', "
					" '" + std::to_string(i) + "', "
					" '" + c[0] + "', "
					" '" + c[1] + "', "
					" 0 );";
			}
		}

		updateBooking();
		tgbtn_classic().IsChecked(false);
		tgbtn_executive().IsChecked(false);
		tgbtn_honeymoon().IsChecked(false);
		tgbtn_junior().IsChecked(false);
		tgbtn_superior().IsChecked(false);
	}


	Windows::Foundation::IAsyncAction Booking::Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{

		if (lst_book().SelectedItem() != nullptr)
		{

			database db(dbPath());
			string user;
			//string res = ""; 

			db << "SELECT username FROM loggedin;"
				>> user;


			auto res = co_await Dialog(
				L"Deleting the booking",
				L"You are about to remove the selected booking. Are you sure you wan to continue?",
				L"Cancel",
				L"Continue"
			);

			if (res == ContentDialogResult::Primary)
			{
				db << "DELETE FROM booking_room WHERE username = ? AND id = ?;"
					<< user
					<< to_string(unbox_value<hstring>(lst_book().SelectedItem().as<Border>().Tag()));



				lst_book().Items().RemoveAt(lst_book().SelectedIndex());

				if (lst_book().Items() == 0)
					brd_book().Visibility(Visibility::Collapsed);


				Dialog(
					L"Booking deleted",
					L"The selected booking deleted successfully",
					L"Ok"
				);

				updateBooking();
			}


		}
		else
			Dialog(
				L"Booking deletion failed",
				L"No booking selected.",
				L"Ok"
			);

	}


	Windows::Foundation::IAsyncAction Booking::btn_pay_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
	{



#pragma region "Generate Dialog"

		auto title = TextBlock();
		title.Text(L"Choose a payment method first");

		auto marg = Thickness();
		marg.Bottom = 15; marg.Top = 5;



		auto subtitle = TextBlock();
		subtitle.Text(L"Available Payments:");
		subtitle.TextAlignment(TextAlignment::Left);
		subtitle.TextWrapping(TextWrapping::WrapWholeWords);
		subtitle.Margin(marg);

		marg.Bottom = 5;




		auto lst_banks = ListView();


		database db(dbPath());

		string userLogged, passLogged;
		int paymentCount = 0;

		db << "SELECT username, password FROM loggedin;"
			>> tie(userLogged, passLogged);


		db << "SELECT paymentName FROM linked_Cards WHERE username = ? UNION SELECT paymentName FROM linked_eWallets WHERE username = ?;"
			<< userLogged
			<< userLogged
			>> [&](string s)  {
			paymentCount++;
		};
	

		if (paymentCount == 0) {
		    auto res = co_await Dialog(
				L"Payment unsuccessful",
				L"Account doesn't link any payment methods, link eWallet or bank cards first before paying this bookings.",
				L"Ok",
				L"Manage Payment Methods"
			);

			if (res == ContentDialogResult::Primary) {
				Frame().Navigate(winrt::xaml_typename<Payment>(), nullptr, DrillInNavigationTransitionInfo());
				
			}
		}
		else {


			db << "SELECT paymentName FROM linked_Cards WHERE username = ? UNION SELECT paymentName FROM linked_eWallets WHERE username = ?;"
				<< userLogged
				<< userLogged
				>> [lst_banks](string paymentName) {


				string paymentName_low = paymentName.c_str();

				auto btn_grd = Grid();
				auto cldef1 = ColumnDefinition(); cldef1.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
				auto cldef2 = ColumnDefinition(); cldef2.Width(GridLengthHelper::FromValueAndType(1, GridUnitType::Star));
				btn_grd.HorizontalAlignment(HorizontalAlignment::Stretch);
				btn_grd.VerticalAlignment(VerticalAlignment::Stretch);
				btn_grd.ColumnDefinitions().Append(cldef1);
				btn_grd.ColumnDefinitions().Append(cldef2);
				btn_grd.Padding(ThicknessHelper::FromLengths(20, 20, 20, 20));


				transform(paymentName_low.begin(), paymentName_low.end(), paymentName_low.begin(), ::tolower);
				auto bitmapImage = BitmapImage(Uri(L"ms-appx:///Assets/Payment/" + to_hstring(paymentName_low) + L".png"));


				auto img_icon = Image();
				img_icon.HorizontalAlignment(HorizontalAlignment::Left);
				img_icon.Margin(ThicknessHelper::FromLengths(0, 0, 20, 0));
				img_icon.Source(bitmapImage);
				img_icon.Height(25);



				auto btn_txt = TextBlock();
				btn_txt.HorizontalAlignment(HorizontalAlignment::Right);
				btn_txt.Margin(ThicknessHelper::FromLengths(20, 0, 0, 0));
				btn_txt.VerticalAlignment(VerticalAlignment::Center);
				btn_txt.FontSize(14);
				btn_txt.Text(to_hstring(paymentName));



				Grid::SetColumn(img_icon, 0);
				Grid::SetColumn(btn_txt, 1);


				btn_grd.Children().Append(img_icon);
				btn_grd.Children().Append(btn_txt);


				lst_banks.Items().Append(btn_grd);

			};


			auto stack = StackPanel();
			stack.Children().Append(subtitle);
			stack.Children().Append(lst_banks);



			winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
			dialog.Title(title);
			dialog.Width(400);

			dialog.Content(stack);
			dialog.CloseButtonText(L"Cancel");
			dialog.PrimaryButtonText(L"Continue");

#pragma endregion



			auto res_1 = co_await dialog.ShowAsync();

			if (res_1 == ContentDialogResult::Primary && lst_banks.SelectedItem() != nullptr)
			{
				auto str = to_hstring(random_otp(6));
				pass_popup().Content(box_value(str));


#pragma region "Generate Dialog"

				auto title = TextBlock();
				title.Text(L"Insert password and OTP");

				auto marg = Thickness();
				marg.Bottom = 15; marg.Top = 5;

				auto subtitle = TextBlock();
				subtitle.Text(L"In order to proceed, please enter the account password and OTP.\nOTP will be received later, please wait before leaving.");
				subtitle.Width(300);
				subtitle.TextWrapping(TextWrapping::WrapWholeWords);
				subtitle.Margin(marg);

				marg.Bottom = 5;

				auto acctPass = PasswordBox();
				acctPass.PlaceholderText(L"Password");
				acctPass.Margin(marg);

				auto otp = PasswordBox();
				otp.PlaceholderText(L"6 character OTP");
				otp.Margin(marg);

				auto stack = StackPanel();
				stack.Children().Append(subtitle);
				stack.Children().Append(acctPass);
				stack.Children().Append(otp);


				winrt::Windows::UI::Xaml::Controls::ContentDialog dialog_1;
				dialog_1.Title(title);


				dialog_1.Content(stack);
				dialog_1.CloseButtonText(L"Cancel");
				dialog_1.PrimaryButtonText(L"Confirm");


				ShowRand();
				auto res_2 = co_await dialog_1.ShowAsync();



				if (res_2 == ContentDialogResult::Primary &&
					acctPass.Password() == to_hstring(passLogged) &&
					otp.Password() == str
					) {
					for (int i = 0; i < lst_book().Items().Size(); i++)
					{
						auto id = unbox_value<hstring>(lst_book().Items().GetAt(i).as<Border>().Tag());

						db << "UPDATE booking_room SET payed = ? WHERE id = ?;"
							<< 1
							<< to_string(id);
					}
					Dialog(
						L"Payment successful",
						L"The payment successfully processed.",
						L"Ok"
					);
					updateBooking();
				}
				else Dialog(
					L"Payment unsuccessful",
					L"The payment cancelled either password or OTP not matched.",
					L"Ok"
				);
#pragma endregion 

			}


		}


	}


	//source: https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
	std::string Booking::random_otp(const int len) {
		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		std::string tmp_s;
		tmp_s.reserve(len);

		for (int i = 0; i < len; ++i) {
			tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
		}

		return tmp_s;
	}


	Windows::Foundation::IAsyncAction Booking::ShowRand() {

		co_await Windows::System::Threading::ThreadPool::RunAsync([&](Windows::Foundation::IAsyncAction const& workItem)
			{
				Sleep(2000);
			});
		pass_popup().IsOpen(true);
	}


	Windows::Foundation::IAsyncAction Booking::pass_popup_ActionButtonClick(winrt::Microsoft::UI::Xaml::Controls::TeachingTip const& sender, winrt::Windows::Foundation::IInspectable const& args)
	{
		//copy text


		winrt::Windows::ApplicationModel::DataTransfer::DataPackage dataPackage = winrt::Windows::ApplicationModel::DataTransfer::DataPackage::DataPackage();
		dataPackage.SetText(unbox_value<hstring>(pass_popup().Content()));
		winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dataPackage);


		copy_popup().IsOpen(true);


		co_await Windows::System::Threading::ThreadPool::RunAsync([&](Windows::Foundation::IAsyncAction const& workItem)
			{
				Sleep(2000);
			});
		pass_popup().IsOpen(false);
		copy_popup().IsOpen(false);
	}

}



