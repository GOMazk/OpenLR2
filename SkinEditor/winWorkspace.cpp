#include "imgui/imgui.h"
#include "../LR2/LR2_skinmanage.h"
#include "../LR2/LR2_skinload.h"
#include "../LR2/LR2_skindraw.h"
#include "../LR2/En_fileutil.h"
#include "../LR2/Scene07_Skinselect.h"
#include "DxLib/DxLib.h"
#include "winWorkspace.h"

#include "seWindowManager.h"
#include "ImageLoader.h"
#include "imgui/TextEditor.h"

#include "skin.h"

const char* SKINTYPESTR[]= {
    "7KEYS",
    "5KEYS",
    "14KEYS",
    "10KEYS",
    "9KEYS",
    "SELECT",
    "DECIDE",
    "RESULT",
    "KEYCONFIG",
    "SKINSELECT",
    "SOUNDSET",
    "THEME",
    "7KEYSBATTLE",
    "5KEYSBATTLE",
    "9KEYSBATTLE",
    "COURSERESULT",
    "OPENING",
    "MODESELECT",
    "MODEDECIDE",
    "COURSESELECT",
    "COURSEEDIT"
}; 
extern SDL_Renderer* renderer;


///////////////////
int WORKSPACE::proc() {

    return 0;
}
int WORKSPACE::init() {

    return 0;
}
int WORKSPACE::draw() {
    ImGui::Begin(title, &alive, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", NULL, &wSkinList)) {
                ScanSkins();
            }
            if (loaded) {
                ImGui::MenuItem("Save as", NULL, &wSaveMenu);
            }
            ImGui::EndMenu();
        }
        if (loaded) {
            
            if (ImGui::BeginMenu("Windows")) {
                //HOW TO ADD FEATURE - STEP 3 : add menu and flag here
                ImGui::MenuItem("Text Editor", NULL, &wTextEdit);
                ImGui::MenuItem("Preview", NULL, &wPreview);
                ImGui::MenuItem("Customize", NULL, &wCustomize);
                if (ImGui::MenuItem("ImgManager", NULL, &wImgManager)) { loadSRC(); };
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("%s", loaded? "" : "no skin loaded");

    ImGui::End();

    //subwindows
    //HOW TO ADD FEATURE - STEP 4 : call function
    if (wSkinList) drawSkinList();

    if (wSaveMenu) drawSaveMenu();

    if (wTextEdit) drawTextEdit();
    if (wPreview) drawPreview();
    if (wCustomize) drawCustomize();
    if (wImgManager) drawImgManager();



    return 0;
}

int WORKSPACE::ScanSkins() {
    InitSkinData(&g.skinData);
    MakeSkinList(&g.skinData, CSTR("LR2files\\Theme\\"));
    MakeSkinList(&g.skinData, CSTR("LR2files\\Sound\\"));
    return 1;
}

int WORKSPACE::drawSkinList() {
    static int item_selected_idx = 0;
    int oldSelected = item_selected_idx;
    static SDL_Texture* preview_tex;
    static int preview_size_x, preview_size_y;
    static bool isPreview = false;

    ImGui::Begin("Open from skinlist", &wSkinList);

    if (ImGui::BeginChild(ImGuiChildFlags_FrameStyle, { 300,-1 }))
    {
        for (int n = 0; n < g.skinData.Count; n++)
        {
            const bool is_selected = (item_selected_idx == n);
            char itemname[260];
            snprintf(itemname, sizeof(itemname), "%02d:%s -%s", n, g.skinData.Data[n].title.outstr(), SKINTYPESTR[g.skinData.Data[n].type]);
            if (ImGui::Selectable(itemname, is_selected)) {
                item_selected_idx = n;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
    }
    ImGui::EndChild();

    int& m = item_selected_idx;
    ImGui::SameLine(0, 0);
    ImGui::BeginGroup();
    //ImTextureRef preview_texid = io.Fonts->TexRef;

    if (oldSelected != m) {
        isPreview = LoadTextureFromFile(g.skinData.Data[m].thumbnail.outstr(), renderer, &preview_tex, &preview_size_x, &preview_size_y);
        oldSelected = m;
    }
    ImGui::Image(isPreview ? preview_tex : NULL, { 320, 240 }, { 0, 0 }, { 1, 1 });
    ImGui::Text("%s", g.skinData.Data[m].skinFile.outstr());
    ImGui::Text("%s", g.skinData.Data[m].title.outstr());
    ImGui::Text("%s", g.skinData.Data[m].maker.outstr());
    ImGui::Text("%s", SKINTYPESTR[g.skinData.Data[m].type]);
    ImGui::Text("%d * %d", g.skinData.Data[m].targetX, g.skinData.Data[m].targetY);

    if (ImGui::Button("LOAD", { 0, 0 })) {
        meta = g.skinData.Data[m];
        snprintf(title, 260, "%s -%s", meta.title.outstr(), SKINTYPESTR[meta.type]);

        strncpy(mainpath, g.skinData.Data[m].skinFile.outstr(), MAX_PATH);
        LoadSkin(mainpath);
        wSkinList = false;
        loaded = true;
    }

    ImGui::SameLine(0, 3);
    ImGui::Button("CLONE", { 0, 0 });
    ImGui::SameLine(0, 3);
    ImGui::Button("BROWSE", { 0, 0 });
    ImGui::SameLine(0, 0);
    ImGui::EndGroup();
    ImGui::End();

    return 0;
}

int WORKSPACE::LoadSkin2(char* path) {
    FILE* pFile;

    pFile = fopen(path, "rb");
    if (pFile) {
        int c = 0;
        
        SKINFILELINEREAD& readS = ((SKINFILELINEREAD*)skinfileLines.data)[skinfileLines.count];
        readS.line.resize(1024);
        sprintf(readS.line, "$FILE %s start", path);
        readS.isComment = true;
        readS.numTotal = skinfileLines.count;
        readS.num = 0;
        skinfileLines.count++;

        while (1) {
            int i = skinfileLines.count;
            SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[i];
            read.line.resize(1024);
            read.filename.resize(260);

            if (fgets(read.line.outstr(), 1023, pFile) == 0) break;
            DealWhiteSpace(&read.line);
            read.numTotal = i;
            read.filename.assign(path);
            read.num = c;
            read.isComment = (*read.line.atPos(0) != '#');

            if(!read.isComment) SplitCSV(read.line, &read.csv, ",");
            
            skinfileLines.count++;
            c++;
            if (skinfileLines.count == skinfileLines.bufSize) skinfileLines.Realloc(skinfileLines.bufSize + 1000);

            SKINFILELINEREAD& read2 = ((SKINFILELINEREAD*)skinfileLines.data)[i];
            if (read2.line.left(8).isSame("#INCLUDE")) {
                if (read2.csv.str[1].canOpenFile()) {
                    LoadSkin2(read2.csv.str[1]);
                }
            }
        }
        SKINFILELINEREAD& readE = ((SKINFILELINEREAD*)skinfileLines.data)[skinfileLines.count];
        readE.line.resize(1024);
        sprintf(readE.line, "$FILE %s end", path);
        readE.isComment = true;
        readE.numTotal = skinfileLines.count;
        readE.num = 0;
        skinfileLines.count++;
        fclose(pFile);
    }

    return 0;
}

int WORKSPACE::LoadSkin(char* path) {
    //FILE* pFile;
    //pFile = fopen(path, "rb");
    //if (pFile) {
    //    fseek(pFile, 0, SEEK_END);
    //    filedatasize = ftell(pFile);

    //    if (filedata) free(filedata);
    //    filedata = (byte*)malloc(filedatasize + 1);//we need null to work
    //    if (filedata) {
    //        memset(filedata, 0, filedatasize + 1);
    //        fseek(pFile, 0, SEEK_SET);
    //        fread(filedata, 1, filedatasize, pFile);
    //    }

    //    fclose(pFile);
    //}

    skinfileLines.Free();
    skinfileLines.Alloc(sizeof(SKINFILELINEREAD), 1000);
    LoadSkin2(path);

    LR2SEInit(&g);

    for (int i = 0; i < 200; i++) g.skstruct.caption[i].fillzero();
    for (int i = 0; i < 10; i++) g.skstruct.helpfilePath[i].fillzero();
    for (int i = 0; i < 20; i++) g.skstruct.customfileRANDOM[i].fillzero();
    for (int i = 0; i < 20; i++) g.skstruct.customfile[i].fillzero();
    g.skstruct.skinMD5.fillzero();
    g.skstruct.skFontname.fillzero();
    for (int i = 0; i < 200; i++) g.skstruct.caption[i].fillzero();
    for (int i = 0; i < 200; i++) g.skstruct.caption[i].assign("(null)");
    for (int i = 0; i < 200; i++) g.skstruct.GrHandle[i] = -1;
    for (int i = 0; i < 10; i++) g.skstruct.helpfilePath[i].fillzero();
    for (int i = 0; i < 10; i++) g.skstruct.helpfilePath[i].assign("(null)");
    //g.skstruct.skFontname.assign(&config.skin.fontname);
    //g.skstruct.disableimagefont = (config.skin.disableimagefont != 0);
    g.skstruct.skinMD5.fillzero();
    g.skstruct.skinMD5.resize2(34);
    AllocDrawingBuffer(&g.skstruct.drBuf);
    LoadScene(&g.skstruct, mainpath, 0, 0);


    SetWindowUserCloseEnableFlag(0);

    //previewScreen = MakeScreen(640, 480);
    //SetDrawScreen(previewScreen);
    //previewScreen = MakeSoftImage(640, 480);
    previewScreen = MakeARGB8ColorSoftImage(640, 480);

    return 0;
}


int WORKSPACE::drawCustomize() {
    char title[260];
    snprintf(title, sizeof(title), "Customize##%d", num);
    ImGui::Begin(title, &wCustomize);

    for (int i = 0; i < meta.custom_count; i++) {
        SkinCustom& cu = meta.customs[i];

        ImGui::TextUnformatted(cu.title);
        ImGui::SameLine();
        
        char item[64];

        if(cu.dst_op_start)
            snprintf(item, sizeof(item), "%03d : %s", cu.dst_op_start + cu.dst_op_selected, cu.op_label[cu.dst_op_selected].outstr());
        else
            snprintf(item, sizeof(item), "FILE : %s", cu.op_label[cu.dst_op_selected].outstr());

        char label[32];
        snprintf(label, sizeof(label), "##%d", i);
        if (ImGui::BeginCombo(label, item, ImGuiComboFlags_WidthFitPreview))
        {
            for (int n = 0; n < cu.dst_op_count; n++)
            {
                if (cu.dst_op_start)
                    snprintf(item, sizeof(item), "%03d : %s", cu.dst_op_start + n, cu.op_label[n].outstr());
                else
                    snprintf(item, sizeof(item), "FILE : %s", cu.op_label[n].outstr());

                const bool is_selected = (cu.dst_op_selected == n);
                if (ImGui::Selectable(item, is_selected))
                    cu.dst_op_selected = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    
    ImGui::End();
    return 0;
}

int WORKSPACE::drawPreview() {
    static SDL_Texture* preview_tex;
    
    char title[260];
    snprintf(title, sizeof(title), "Preview##%d", num);

    LR2SEDrawLoop(&g, previewScreen);
    
    ImGui::Begin(title, &wPreview);
    LoadTextureFromRawMemory(GetImageAddressSoftImage(previewScreen), renderer, &preview_tex, 640, 480, 4);
    ImGui::Image(preview_tex, { 640, 480 }, { 0, 0 }, { 1, 1 });

    if (ImGui::Button("Start")) {
        LR2SESceneInit(&g, meta.type);
    }
    LR2SESceneProc(&g, meta.type);

    //D_IDirect3DSurface9* d9 = (D_IDirect3DSurface9*)GetUseDirect3D9BackBufferSurface();
    //d9->GetDC()

    ImGui::End();

    if (!wPreview) SetWindowVisibleFlag(0);
    return 0;
}

int WORKSPACE::drawTextEdit() {
    char title[260];
    snprintf(title, sizeof(title), "TextEdit##%d", num);

    ImGui::Begin(title, &wTextEdit, ImGuiWindowFlags_MenuBar);

    //TEMP
    static bool hideComment = false;
    static bool hideBlank = false;

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Hide")) {

            ImGui::MenuItem("Blank", NULL, &hideBlank);
            ImGui::MenuItem("Comment", NULL, &hideComment);

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }


    for (int n = 0; n < skinfileLines.count; n++) {
        SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[n];
        /*char itemname[260];
        snprintf(itemname, sizeof(itemname), "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());*/
        ImVec4 color;
        color = {1.0f, 1.0f, 1.0f, 1.0f};

        if (hideBlank && *read.line.atPos(0) == '\0') continue;
        if (hideComment && read.isComment) continue;

        ImGui::PushID(n);
        ImGui::Button(" ");
        ImGui::PopID();

        ImGui::SameLine();


        if (read.isComment) {
            ImGui::TextDisabled("%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());
            //ImGui::TextColored(color, "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());
        }
        else {
            //ImGui::TextColored(color, "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());
            ImGui::TextColored(color, "%04d:%04d: ", read.numTotal, read.num);
            ImGui::SameLine();
            char tablename[260];
            snprintf(tablename, sizeof(tablename), "##w%d_text", num);
            if (ImGui::BeginTable(tablename, 22, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody))
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 22; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if(read.csv.str[column].atPos(0) == nullptr)
                        ImGui::TextDisabled("%s", read.csv.str[column].body);
                    else
                        ImGui::TextUnformatted(read.csv.str[column]);
                }
                ImGui::EndTable();
            }
        }
    }

    /*if (ImGui::BeginChild(ImGuiChildFlags_FrameStyle))
    {
        for (int n = 0; n < skinfileLines.count; n++){
            SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[n];

            const bool is_selected = 0;
            char itemname[260];
            snprintf(itemname, sizeof(itemname), "%04d:%04d: %s", read.numTotal, read.num, read.line.outstr());
            ImGui::Selectable(itemname, is_selected);
        }
    }
    ImGui::EndChild();*/




    ImGui::End();
    return 0;
}

int WORKSPACE::loadSRC() {



    for (int n = 0; n < skinfileLines.count; n++) {
        SKINFILELINEREAD& read = ((SKINFILELINEREAD*)skinfileLines.data)[n];
        if (read.line.left(10).isSame("#SRC_IMAGE")) {
            
        }
    }



    return 0;
}

int WORKSPACE::drawImgManager() {
    char title[260];
    snprintf(title, sizeof(title), "ImageManager##%d", num);
    ImGui::Begin(title, &wImgManager);




    ImGui::End();
    return 0;
}


//TODO apply split
int WORKSPACE::SaveSkinScript(char* path, bool split, bool nocomment) {

    FILE* pFile;
    pFile = fopen(path, "wb");
    if (pFile == NULL) return -1;
    for (int i = 0; i < skinfileLines.count; i++) {
        if (nocomment && ((SKINFILELINEREAD*)skinfileLines.data)[i].isComment) continue;

        fputs(((SKINFILELINEREAD*)skinfileLines.data)[i].line, pFile);
        fputs("\n", pFile);
    }
    fclose(pFile);
    return 0;
}

int WORKSPACE::drawSaveMenu() {
    char title[260], input[32], result[32];
    snprintf(title, sizeof(title), "SaveMenu##%d", num);
    snprintf(input, sizeof(input), "##savePathInput%d", num);

    if(ImGui::Begin(title, &wSaveMenu, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)) {
        static int split, nocomment, exist, success;
        static bool wSaveMenuResult;

        ImGui::Text("old path is %s ", mainpath);
        static char newPath[260] = "";
        if (newPath[0] == '\0') { //very init
            strncpy(newPath, mainpath, sizeof(newPath));
        }

        if (ImGui::Button("BROWSE", { 0, 0 })) {
            //TODO
        }
        ImGui::SameLine(0, 0);
        ImGui::InputText(input, newPath, IM_ARRAYSIZE(newPath));
        exist = IsFileExist(newPath) || !strcmp(mainpath, newPath); //TODO reduce cpu usage

        if (exist) ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "CAUTION: OVERWRITE");
        ImGui::SeparatorText("Script files");
        ImGui::RadioButton("merge scripts", &split, 0);
        ImGui::RadioButton("split scripts", &split, 1);
        ImGui::SeparatorText("Comments");
        ImGui::RadioButton("maintain memo", &nocomment, 0);
        ImGui::RadioButton("delete memo", &nocomment, 1);
        
        if (ImGui::Button("SAVE", { 0, 0 })) {
            success = (SaveSkinScript(newPath, split, nocomment) == 0);
        }

        if (success) {
            snprintf(result, sizeof(result), "SaveResult##Save%d", num);
            if (ImGui::Begin(result, &wSaveMenuResult, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
                ImGui::Text("save success");
                if (ImGui::Button("OK")){
                    wSaveMenuResult = 0;
                    wSaveMenu = 0;
                    success = 0;
                }
                ImGui::End();
            }
        }
        /*if (success) { //why it doesn't work??
            snprintf(result, sizeof(result), "SaveResult##Save%d", num);
            if (ImGui::BeginPopupModal("Save?", &wSaveMenuResult, ImGuiWindowFlags_AlwaysAutoResize)){
                ImGui::Text("save success");
                if (ImGui::Button("OK")) {
                    wSaveMenuResult = 0;
                    wSaveMenu = 0;
                    success = 0;
                }
                ImGui::EndPopup();
            }
        }*/
        ImGui::End();
    }
    return 0;
}
//{
//    CSTR savepath(mainpath);
//    savepath.add("_master");
//    SaveMaster(savepath);
//}

//HOW TO ADD FEATURE - STEP 2 : write function









ARR workspaceList;

int ARR::Alloc(int structsize, int size) {
    if (data) return -1; //already exist

    unitSize = structsize;

    data = malloc(unitSize * size);
    if (!data) return -1; //malloc fail
    bufSize = size;

    memset(data, 0, unitSize * size);
    return 0;
}

int ARR::Realloc(int size) {
    void* temp = realloc(data, unitSize * size);
    if (!temp) return -1;

    data = temp;
    memset((void*)((int)data + unitSize * bufSize), 0, unitSize * (size - bufSize));
    bufSize = size;
    return 0;
}

int ARR::Free() {
    if (data) free(data);
    data = NULL;
    bufSize = 0;
    count = 0;
    return 0;
}

int ARR::push_back(void* newdata, int size) {
    //memcpy( this->data + unitSize * count, newdata, size);
    return 0;
}
