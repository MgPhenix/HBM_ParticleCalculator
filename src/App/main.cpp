#include <iostream>
#include "main.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL.h"

#include "Initialize.h"
#include "vector"
#include "unordered_map"

enum class EParticle {
	CopperIon,
	HydrogenIon,
	LeadIon,
	Antimatter,
	Antischrabidium,
	Muon,
	HiggsBoson,
	Tachyon,
	DarkMatter,
	StrangeQuark,
	Sparkticle,
	Digamma,

	None
};

static std::unordered_map<EParticle, std::string> converter{
	{EParticle::CopperIon, "Copper Ion"},
	{EParticle::HydrogenIon, "Hydrogen Ion"},
	{EParticle::LeadIon, "Lead Ion"},
	{EParticle::Antimatter, "Antimatter"},
	{EParticle::Antischrabidium, "Antischrabidium"},
	{EParticle::Muon, "Muon"},
	{EParticle::HiggsBoson, "Higgs Boson"},
	{EParticle::Tachyon, "Tachyon"},
	{EParticle::DarkMatter, "Dark Matter"},
	{EParticle::StrangeQuark, "Strange Quark"},
	{EParticle::Sparkticle, "Sparkticle"},
	{EParticle::Digamma, "Digamma"},
};

struct IParticle 
{
	EParticle type = EParticle::None;

	IParticle(EParticle t) : type(t) {};
	IParticle() = default;

	std::vector<std::pair<int, EParticle>> component;
	
	void AddParticle(int price, EParticle particle) 
	{ 
		component.push_back({ price, particle }); 
	};


	std::unordered_map<EParticle, int> GetPrice(std::unordered_map<EParticle, IParticle*>& particleList)
	{
		std::unordered_map<EParticle, int> map;

		for (auto& particle : component)
		{
			map[particle.second] += particle.first;
			std::unordered_map<EParticle, int> map2 = particleList[particle.second]->GetPrice(particleList);
			
			for (auto& pair : map2)
			{
				map[pair.first] += map2[pair.first] * particle.first;
			}
		}

		return map;
	}
};


std::unordered_map<EParticle, IParticle*> CreateList()
{
	std::unordered_map<EParticle, IParticle*> particleList;

	particleList[EParticle::CopperIon]		 = new IParticle(EParticle::CopperIon);
	particleList[EParticle::HydrogenIon]	 = new IParticle(EParticle::HydrogenIon);
	particleList[EParticle::LeadIon]		 = new IParticle(EParticle::LeadIon);
	particleList[EParticle::Antimatter]		 = new IParticle(EParticle::Antimatter);
	particleList[EParticle::Antischrabidium] = new IParticle(EParticle::Antischrabidium);
	particleList[EParticle::Muon]			 = new IParticle(EParticle::Muon);
	particleList[EParticle::HiggsBoson]		 = new IParticle(EParticle::HiggsBoson);
	particleList[EParticle::Tachyon]		 = new IParticle(EParticle::Tachyon);
	particleList[EParticle::DarkMatter]		 = new IParticle(EParticle::DarkMatter);
	particleList[EParticle::StrangeQuark]	 = new IParticle(EParticle::StrangeQuark);
	particleList[EParticle::Sparkticle]		 = new IParticle(EParticle::Sparkticle);
	particleList[EParticle::Digamma]		 = new IParticle(EParticle::Digamma);


	particleList[EParticle::Antimatter]->AddParticle(1, EParticle::CopperIon);
	particleList[EParticle::Antimatter]->AddParticle(1, EParticle::HydrogenIon);

	particleList[EParticle::Antischrabidium]->AddParticle(2, EParticle::Antimatter);

	particleList[EParticle::Muon]->AddParticle(1, EParticle::Antimatter);
	particleList[EParticle::Muon]->AddParticle(1, EParticle::HydrogenIon);

	particleList[EParticle::HiggsBoson]->AddParticle(1, EParticle::HydrogenIon);
	particleList[EParticle::HiggsBoson]->AddParticle(1, EParticle::LeadIon);

	particleList[EParticle::Tachyon]->AddParticle(1, EParticle::Muon);
	particleList[EParticle::Tachyon]->AddParticle(1, EParticle::HiggsBoson);

	particleList[EParticle::DarkMatter]->AddParticle(2, EParticle::Antischrabidium);

	particleList[EParticle::StrangeQuark]->AddParticle(1, EParticle::DarkMatter);
	particleList[EParticle::StrangeQuark]->AddParticle(1, EParticle::Muon);

	particleList[EParticle::Sparkticle]->AddParticle(1, EParticle::StrangeQuark);

	particleList[EParticle::Digamma]->AddParticle(1, EParticle::Sparkticle);
	particleList[EParticle::Digamma]->AddParticle(1, EParticle::HiggsBoson);

	return particleList;
}







int main()
{

	//INIT
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int windW = 800;
	int windH = 800;

	if (!InitSDL3("HBM Particle Calculator", windW, windH, &window, &renderer))
	{
		return 0;
	}

	ImGuiIO& io = InitImGui(window, renderer);


	std::unordered_map<EParticle, IParticle*> map = CreateList();

	//LOOP 

	int nbr = 1;
	bool ShowResultTab = false;
	std::unordered_map<EParticle, int> result;
	EParticle selected = EParticle::None;

	bool run = true;
	while (run)
	{
		//Event
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				run = false;

			if (event.type == SDL_EVENT_WINDOW_RESIZED)
				SDL_GetWindowSize(window, &windW, &windH);

			ImGui_ImplSDL3_ProcessEvent(&event);
		}


		//Frame
		ImGui::SDL3_NewFrame();


		// DO shitty thing
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(windW, windH));

		ImGui::Begin("Main", NULL,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoDecoration
		);

		ImGui::Text("Calculator");
		ImGui::Separator();
		ImGui::NewLine();

		for (auto& pair : converter)
		{
			ImGui::SameLine();
			if (ImGui::Button(pair.second.c_str()))
			{
				selected = pair.first;
			}
		}


		ImGui::SliderInt("Count", &nbr, 1, 64);

		if (ImGui::Button("Show Result"))
		{
			ShowResultTab = !ShowResultTab;
			
		}

		if (selected != EParticle::None)
			result = map[selected]->GetPrice(map);

		ImGui::Separator();
		ImGui::NewLine();

		if (ShowResultTab)
		{
			if (result.empty())
			{
				ImGui::Text("No Recipe in Particle Accelerator or something went wrong");
			}
			else if (ImGui::BeginTable("EntityTable", 2,ImGuiTableFlags_Borders))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Particle");
				ImGui::TableNextColumn();
				ImGui::Text("Count");

				for (auto& pair : result)
				{
					int value = pair.second * nbr;
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text(converter[pair.first].c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%d", value);
				}

				ImGui::EndTable();
			}
		}

		ImGui::End();


		//Render SDL
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderClear(renderer);


		//Render ImGui
		ImGui::SDL3_ImGuiRender(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}