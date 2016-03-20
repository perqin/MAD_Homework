using MAD_HW4.ViewModels;
using System;
using Windows.ApplicationModel;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;

namespace MAD_HW4 {
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class EditPage : Page {
        private TodoViewModel TodoVM;
        private Todo displayTodo = new Todo();

        public Todo DisplayTodo {
            get {
                return displayTodo;
            }
        }

        public EditPage() {
            InitializeComponent();
            Application.Current.Resuming += App_Resuming;
            Application.Current.Suspending += App_Suspending;
            TodoVM = TodoViewModel.getInstance();
        }

        public void UpdateView(Todo todo) {
            displayTodo.Title = todo.Title;
            displayTodo.Detail = todo.Detail;
            displayTodo.DueDate = todo.DueDate;
            displayTodo.Done = todo.Done;
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
            //TODO: Restore current edit
        }

        private void App_Suspending(object sender, SuspendingEventArgs e)
        {
            //TODO: Store current edit
        }
    }
}
