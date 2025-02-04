#pragma once
#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
#endif
#pragma push_macro("OVERRIDE")
#undef OVERRIDE // cef headers provide their own OVERRIDE macro
THIRD_PARTY_INCLUDES_START
#include "include/cef_client.h"
#include "include/cef_app.h"
THIRD_PARTY_INCLUDES_END
#pragma pop_macro("OVERRIDE")
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"
#endif
#include "BluEye.generated.h"

class BrowserClient;
class RenderHandler;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScriptEvent, const FString&, EventName, const FString&, EventMessage);

struct FBluTextureParams
{

	// Pointer to our Texture's resource
	FTexture2DResource* Texture2DResource;

};

struct FUpdateTextureRegionsData
{
	FTexture2DResource * Texture2DResource;
	uint32 NumRegions;
	FUpdateTextureRegion2D * Regions;
	uint32 SrcPitch;
	uint32 SrcBpp;
	uint8 * SrcData;
};

UENUM(BlueprintType)
enum EBluSpecialKeys
{ 
	backspacekey = 8 UMETA(DisplayName = "Backspace"),
	tabkey = 9 UMETA(DisplayName = "Tab"),
	enterkey = 13 UMETA(DisplayName = "Enter"),
	pausekey = 19 UMETA(DisplayName = "Pause"),
	escapekey = 27 UMETA(DisplayName = "Escape"),
	pageupkey = 33 UMETA(DisplayName = "Page Up"),
	pagedownkey = 34 UMETA(DisplayName = "Page Down"),
	endkey = 35 UMETA(DisplayName = "End"),
	homekey = 36 UMETA(DisplayName = "Home"),
	leftarrowkey = 37 UMETA(DisplayName = "Left Arrow"),
	rightarrowkey = 38 UMETA(DisplayName = "Right Arrow"),
	downarrowkey = 39 UMETA(DisplayName = "Down Arrow"),
	uparrowkey = 40 UMETA(DisplayName = "Up Arrow"),
	insertkey = 45 UMETA(DisplayName = "Insert"),
	deletekey = 46 UMETA(DisplayName = "Delete"),
	numlockkey = 144 UMETA(DisplayName = "Num Lock"),
	scrolllockkey = 145 UMETA(DisplayName = "Scroll Lock")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDownloadCompleteSignature, FString, url);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDownloadUpdatedSignature, FString, url, float, percentage);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDownloadComplete);

UCLASS(ClassGroup = Blu, Blueprintable)
class BLU_API UBluEye : public UObject
{
	GENERATED_BODY()

	UBluEye(const class FObjectInitializer& PCIP);

public:

	//Event delegates
	UPROPERTY(BlueprintAssignable, Category = "Blu Browser Events")
		FDownloadCompleteSignature DownloadComplete;

	UPROPERTY(BlueprintAssignable, Category = "Blu Browser Events")
		FDownloadUpdatedSignature DownloadUpdated;

	//GENERATED_UCLASS_BODY()

	/** Initialize function, should be called after properties are set */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void init();

	/** The default URL this UI component will load */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		FString DefaultURL;

	/** Is this UI component current active? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		bool bEnabled;

	/** Should this be rendered in game to be transparent? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		bool bIsTransparent;

	/** Width of the view resolution */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		int32 Width;

	/** Height of the view resolution */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		bool bEnableWebGL;

	/** Material that will be instanced to load UI texture into it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		UMaterialInterface* BaseMaterial;

	/** Name of parameter to load UI texture into material */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blu")
		FName TextureParameterName = "BluTexture";

	UFUNCTION(BlueprintCallable, Category = "Blu")
		UBluEye* SetProperties(const int32 SetWidth,
							const int32 SetHeight,
							const bool SetIsTransparent,
							const bool SetEnabled,
							const bool SetWebGL,
							const FString& SetDefaultURL,
							const FName& SetTextureParameterName,
							UMaterialInterface* SetBaseMaterial);

	/** Get the texture data from our UI component */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		UTexture2D* GetTexture() const;

	/** Execute JS code inside the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void ExecuteJS(const FString& code);

	/** 
	 * Execute a JS function/method by name with FString Array as params.
	 * Each element in the array will be passed into the function in order and separated by a ,
	 * If you want to pass a JSON string as an object, simply don't put quotes around the outside braces {"foo" : "bar"}
	 * If you want to pass a number, do similar: 10.5
	 * To pass as a string, place quotes around the param when adding to the array: "10.5" and "hello" are strings
	 */
	UFUNCTION(BlueprintCallable, Category = "Blu", meta = (DisplayName = "Execute Javascript With Params", Keywords = "js javascript parameters"))
		void ExecuteJSMethodWithParams(const FString& methodName, const TArray<FString> params);

	/** Load a new URL into the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void LoadURL(const FString& newURL);

	/** Get the currently loaded URL */
	UFUNCTION(BlueprintPure, Category = "Blu")
		FString GetCurrentURL();

	/** Trigger Zoom */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void SetZoom(const float scale = 1);

	/** Get our zoom level */
	UFUNCTION(BlueprintPure, Category = "Blu")
		float GetZoom();

	//Not ready yet
	//UFUNCTION(BlueprintCallable, Category = "Blu Test")
		void Test();

	/** Download a file */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void DownloadFile(const FString& fileUrl);

	/** Trigger a LEFT click in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerLeftClick(const FVector2D& pos, const float scale = 1);

	/** Trigger a RIGHT click in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerRightClick(const FVector2D& pos, const float scale = 1);

	/** Trigger a LEFT MOUSE DOWN in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerLeftMouseDown(const FVector2D& pos, const float scale = 1);

	/** Trigger a RIGHT MOUSE DOWN in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerRightMouseDown(const FVector2D& pos, const float scale = 1);

	/** Trigger a LEFT MOUSE UP in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerLeftMouseUp(const FVector2D& pos, const float scale = 1);

	/* Trigger a RIGHT MOUSE UP in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerRightMouseUp(const FVector2D& pos, const float scale = 1);

	/** Move the mouse in the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerMouseMove(const FVector2D& pos, const float scale = 1);

	/** Move the mouse in the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerMouseWheel(const float MouseWheelDelta, const FVector2D& pos, const float scale = 1);

	/** Javascript event emitter */
	UPROPERTY(BlueprintAssignable)
		FScriptEvent ScriptEventEmitter;

	/** Trigger a key down event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void KeyDown(FKeyEvent InKey);

	/** Trigger a key up event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void KeyUp(FKeyEvent InKey);

	/** Trigger a key press event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void KeyPress(FKeyEvent InKey);

	/** Trigger a character key event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void CharKeyPress(FCharacterEvent CharEvent);

	/** Trigger a raw keypress via a character */
	UFUNCTION(BlueprintCallable, Category = "Blu", meta = (AdvancedDisplay = "2"))
		void RawCharKeyPress(const FString charToPress, bool isRepeat,
								bool LeftShiftDown,
								bool RightShiftDown,
								bool LeftControlDown,
								bool RightControlDown,
								bool LeftAltDown,
								bool RightAltDown,
								bool LeftCommandDown,
								bool RightCommandDown,
								bool CapsLocksOn);

	UFUNCTION(BlueprintCallable, Category = "Blu", meta = (AdvancedDisplay = "2"))
		void SpecialKeyPress(EBluSpecialKeys key,
								bool LeftShiftDown,
								bool RightShiftDown,
								bool LeftControlDown,
								bool RightControlDown,
								bool LeftAltDown,
								bool RightAltDown,
								bool LeftCommandDown,
								bool RightCommandDown,
								bool CapsLocksOn);

	/** Close the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void CloseBrowser();

	/** Check if the browser is still loading */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		bool IsBrowserLoading();

	/** Reloads the browser's current page */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void ReloadBrowser(bool IgnoreCache);

	/** Navigate back in this web view's history */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void NavBack();

	/** Navigate forward in this web view's history */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void NavForward();

	/** Resize the browser's viewport */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		UTexture2D* ResizeBrowser(const int32 NewWidth, const int32 NewHeight);

	//This cropping function doesn't work atm
	//UFUNCTION(BlueprintCallable, Category = "Blu")
		UTexture2D* CropWindow(const int32 Y, const int32 X, const int32 NewWidth, const int32 NewHeight);

	CefRefPtr<CefBrowser> browser;

	void TextureUpdate(const void* buffer, FUpdateTextureRegion2D * updateRegions, uint32  regionCount);

	void BeginDestroy() override;

	protected:
		CefWindowInfo info;
		CefRefPtr<BrowserClient> g_handler;
		CefBrowserSettings browserSettings;
		RenderHandler* renderer;

		void ResetTexture();
		void DestroyTexture();
		void ResetMatInstance();
		
		// Parse UE4 key events, helper
		void processKeyCode(FKeyEvent InKey);

		// Helper for processing key modifiers
		void processKeyMods(FInputEvent InKey);

		// Store UI state in this UTexture2D
		UPROPERTY()
		UTexture2D* Texture;

		UMaterialInstanceDynamic* MaterialInstance;

		CefMouseEvent mouse_event;
		CefKeyEvent key_event;

	private:

		FBluTextureParams RenderParams;

};
class RenderHandler : public CefRenderHandler
{
	public:
		UBluEye* parentUI;

		int32 Width;
		int32 Height;

		// CefRenderHandler interface
		bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

		RenderHandler(int32 width, int32 height, UBluEye* ui);

		// CefBase interface
		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(RenderHandler)
};

// for manual render handler
class BrowserClient : public CefClient, public CefLifeSpanHandler, public CefDownloadHandler, public CefDisplayHandler
{

	private:
		FScriptEvent* event_emitter;
		CefRefPtr<RenderHandler> m_renderHandler;

		// For lifespan
		CefRefPtr<CefBrowser> m_Browser;
		int m_BrowserId;
		bool m_bIsClosing;

	public:
		BrowserClient(RenderHandler* renderHandler) : m_renderHandler(renderHandler)
		{
		
		};

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() 
		{
			return m_renderHandler;
		};

		// Getter for renderer
		virtual CefRefPtr<RenderHandler> GetRenderHandlerCustom()
		{
			return m_renderHandler;
		};

		// Getter for lifespan
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
		{
			return this;
		}

		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
		void SetEventEmitter(FScriptEvent* emitter);

		//CefDownloadHandler
		virtual void OnBeforeDownload(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			const CefString& suggested_name,
			CefRefPtr<CefBeforeDownloadCallback> callback) override;

		virtual void OnDownloadUpdated(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			CefRefPtr<CefDownloadItemCallback> callback) override;

		//CefLifeSpanHandler
		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access) override
		{
			return false;
		}

		// Lifespan methods
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(BrowserClient)
};
