using MAD_HW4.ViewModels;
using System;
using Windows.ApplicationModel;
using Windows.Data.Json;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;

namespace MAD_HW4 {
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class EditPage : Page {
        //private TodoViewModel TodoVM;
        private Todo displayTodo = new Todo();

        public Todo DisplayTodo {
            get {
                return displayTodo;
            }
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            JsonObject parameters = JsonObject.Parse(e.Parameter as string);
            if (parameters != null)
            {
                displayTodo.FromString(parameters["EditingTodoData"].GetString());
            }
        }

        public EditPage() {
            InitializeComponent();
            Application.Current.Resuming += App_Resuming;
            Application.Current.Suspending += App_Suspending;
            //TodoVM = TodoViewModel.getInstance();
        }

        /// <summary>
        /// Change editing todo data without changing ID
        /// </summary>
        /// <param name="todo"></param>
        public void ChangeEditingTodoData(Todo todo) {
            displayTodo = new Todo();
            displayTodo.Title = todo.Title;
            displayTodo.Detail = todo.Detail;
            displayTodo.DueDate = todo.DueDate;
            displayTodo.Done = todo.Done;
            displayTodo.CoverImageExt = todo.CoverImageExt;
            displayTodo.CoverSource = todo.CoverSource;
        }

        private async void SelectCoverButton_Click(object sender, RoutedEventArgs e) {
            FileOpenPicker picker = new FileOpenPicker();
            picker.ViewMode = PickerViewMode.Thumbnail;
            picker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
            picker.FileTypeFilter.Add(".bmp");
            picker.FileTypeFilter.Add(".jpeg");
            picker.FileTypeFilter.Add(".jpg");
            picker.FileTypeFilter.Add(".png");
            StorageFile file = await picker.PickSingleFileAsync();
            if (file != null) {
                using (IRandomAccessStream fileStream = await file.OpenAsync(FileAccessMode.Read)) {
                    BitmapImage bitmapImage = new BitmapImage();
                    await bitmapImage.SetSourceAsync(fileStream);
                    displayTodo.CoverSource = bitmapImage;
                }
            }
        }

        private void App_Resuming(object sender, object e)
        {
            ApplicationData.Current.LocalSettings.Values["EditingTodoData"] = displayTodo.ToString();
        }

        private void App_Suspending(object sender, SuspendingEventArgs e)
        {
            displayTodo.FromString(ApplicationData.Current.LocalSettings.Values["EditingTodoData"] as string);
        }
    }
}
