#pragma once

namespace gui
{

	bool order_summary_open{ false };
	bool insufficient_funds_open{ false };
	bool take_away{ false };
	float amount_tendered;

	const uint16_t width = 1280;
	const uint16_t height = 720;

	void init(GLFWwindow * window)
	{

		//ImGui setup
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.WantSaveIniSettings = false;

		//Set size and pos of the window
		ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ width, height }, ImGuiCond_Once);

		//Set style
		ImGui::StyleColorsLight();
		//Link our OpenGl window to ImGui
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		//Styling
		ImGuiStyle* imgui_style = &ImGui::GetStyle();
		imgui_style->ItemSpacing = ImVec2(12, 8);
		

	}

	void daily_summary()
	{

		ImGui::Text("Daily Statistics");
		ImGui::Separator();

		if (ImGui::BeginTable("dailysummary##table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX))
		{

			//Setup table
			ImGui::TableSetupColumn("Total Cups");
			ImGui::TableSetupColumn("Takeaway Orders");
			ImGui::TableSetupColumn("Dine in Orders");
			ImGui::TableSetupColumn("Total Orders");
			ImGui::TableSetupColumn("Gst Collected");
			ImGui::TableSetupColumn("Income");
			ImGui::TableHeadersRow();

			//Write entries to table
			ImGui::TableNextColumn();
			ImGui::Text("%d", sales_info::total_cups);
			ImGui::TableNextColumn();
			ImGui::Text("%d", sales_info::takeaway_orders);
			ImGui::TableNextColumn();
			ImGui::Text("%d", sales_info::dinein_orders);
			ImGui::TableNextColumn();
			ImGui::Text("%d", sales_info::dinein_orders + sales_info::takeaway_orders);
			ImGui::TableNextColumn();
			ImGui::Text("%.2f", sales_info::gst);
			ImGui::TableNextColumn();
			ImGui::Text("%.2f", sales_info::income);

			ImGui::EndTable();

		}

		if (ImGui::Button("Dump to CSV file"))
		{

			std::ofstream csv_file("daily_summary.csv");

			csv_file << "ORDERS_COUNT,DINE-IN,TAKE-AWAY,CAPPUCCINO_COUNT,ESPRESSO_COUNT,LATTE_COUNT,ICED_COFFEE_COUNT,CUPS_COUNT,GST_COLLECTED,DAILY_INCOME\n";
			csv_file << (sales_info::dinein_orders + sales_info::takeaway_orders) << "," << sales_info::dinein_orders << "," << sales_info::takeaway_orders << ",";

			for (auto& item : menu::menu_items)
			{
				
				csv_file << item.total() << ",";

			}
			
			csv_file << sales_info::total_cups << "," << sales_info::gst << "," << sales_info::income << "\n";

		}

	}

	void order_tab()
	{

		float running_total{ 0 };
		float running_total_gst{ 0 };
		uint16_t total_cups{ 0 };

		ImGui::Checkbox("Take Away", &take_away);

		for ( auto& item  : menu::menu_items)
		{

			const std::string& item_name = item.name();
			std::string item_id = item_name + "button";
			unsigned int &item_quantity = item.order_total();
			unsigned int& total_quantity = item.total();

			ImGui::PushID(item_id.c_str());

			if (ImGui::Button("-", ImVec2(25.f, 25.f)))
			{
				if (item_quantity)
				{
					item_quantity--;
					total_quantity--;
				}
			}				

			ImGui::SameLine(45.f);
			ImGui::Text("%i", item_quantity);
			ImGui::SameLine();

			if (ImGui::Button("+", ImVec2(25.f, 25.f)))
			{
				total_quantity++;
				item_quantity++;
			}
				

			ImGui::SameLine();			
			ImGui::Text(item_name.c_str());
			ImGui::PopID();

			if (take_away)
			{

				running_total += (item_quantity * (item.price() * 1.05));
				running_total_gst += (item_quantity * (item.price_gst() * 1.05));

			}
			else
			{

				running_total += (item_quantity * item.price());
				running_total_gst += (item_quantity * item.price_gst());

			}
			

		}

		ImGui::Text("Order Total (No GST): %.2f", running_total);
		ImGui::Text("Order Total (GST): %.2f", running_total_gst);
		if (ImGui::Button("Order") && running_total)
			order_summary_open = true;

		if (order_summary_open)
		{
			ImGui::SetNextWindowPos({ width / 3, height / 3 }, ImGuiCond_Once);

			if (ImGui::Begin("Order Summary", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{

				if (ImGui::BeginTable("order##table", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX))
				{

					//Table header
					ImGui::TableSetupColumn("Quantity");
					ImGui::TableSetupColumn("Menu Item");
					ImGui::TableSetupColumn("Item Price");
					ImGui::TableSetupColumn("Gst");
					ImGui::TableSetupColumn("Total Price");
					ImGui::TableHeadersRow();

					for (auto& item : menu::menu_items)
					{
						ImGui::TableNextRow();

						ImGui::TableNextColumn();
						ImGui::Text("%d", item.order_total());
						ImGui::TableNextColumn();
						ImGui::Text("%s", item.name().c_str());
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", item.price());
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", item.gst());
						ImGui::TableNextColumn();

						float item_total = item.order_total() * item.price_gst();
						ImGui::Text("%.2f", item_total);

					}

					ImGui::EndTable();

				}

				if (take_away)
					ImGui::Text("5%% surcharge for takeaway order");

				ImGui::Text("Amount Due: %.2f", running_total_gst);
				ImGui::InputFloat("Amount tendered", &amount_tendered, 0, 0, "%.2f");
				ImGui::Text("Change: %.2f", amount_tendered - running_total_gst);

				if (ImGui::Button("OK"))
				{

					if (amount_tendered >= running_total_gst)
					{

						order_summary_open = false;

						//Actually add to the statistics and clear the item orders
						for (auto& item : menu::menu_items)
						{

							unsigned int& item_total = item.order_total();
							sales_info::total_cups += item_total;
							sales_info::gst += (item.gst() * item_total);

							item_total = 0;

						}

						sales_info::income += running_total_gst;


						if (take_away)
							sales_info::takeaway_orders++;
						else
							sales_info::dinein_orders++;

					}

				}

			}

			ImGui::End();

		}

	}

	void render()
	{

		ImGui::Begin("Cafe Au Lait", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
		
		if (ImGui::BeginTabBar("#tabs"))
		{

			if (ImGui::BeginTabItem("Order"))
			{
				order_tab();
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Daily Summary"))
			{
				daily_summary();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();

		}


		ImGui::End();

	}

	void destroy()
	{

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();	

	}

}