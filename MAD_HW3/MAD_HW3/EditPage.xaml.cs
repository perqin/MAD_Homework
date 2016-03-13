using MAD_HW3.ViewModels;
using System;
using Windows.Storage.Pickers;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;

namespace MAD_HW3
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class EditPage : Page
    {
        private TodoViewModel TodoVM;
        private Todo displayTodo = new Todo();

        public Todo DisplayTodo
        {
            get
            {
                return displayTodo;
            }
        }

        public EditPage()
        {
            this.InitializeComponent();

            TodoVM = TodoViewModel.getInstance();
        }

        public void UpdateView(Todo todo)
        {
            displayTodo.Title = todo.Title;
            displayTodo.Detail = todo.Detail;
            displayTodo.DueDate = todo.DueDate;
            displayTodo.Done = todo.Done;
            displayTodo.CoverSource = todo.CoverSource;
            //RefreshView();
        }

        private async void SelectCoverButton_Click(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            FileOpenPicker picker = new FileOpenPicker();
            picker.ViewMode = PickerViewMode.Thumbnail;
            picker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
            picker.FileTypeFilter.Add(".bmp");
            picker.FileTypeFilter.Add(".jpeg");
            picker.FileTypeFilter.Add(".jpg");
            picker.FileTypeFilter.Add(".png");
            StorageFile file = await picker.PickSingleFileAsync();
            if (file != null)
            {
                using (IRandomAccessStream fileStream = await file.OpenAsync(FileAccessMode.Read))
                {
                    BitmapImage bitmapImage = new BitmapImage();
                    await bitmapImage.SetSourceAsync(fileStream);
                    displayTodo.CoverSource = bitmapImage;
                }
            }
        }

        //private void RefreshView()
        //{
        //
        //}
    }
}
