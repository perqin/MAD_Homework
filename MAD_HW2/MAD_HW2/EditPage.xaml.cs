using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Core;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;

namespace MAD_HW2
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class EditPage : Page
    {
        public EditPage()
        {
            this.InitializeComponent();
            
            SystemNavigationManager.GetForCurrentView().BackRequested += App_BackRequested;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            Frame rootFrame = Window.Current.Content as Frame;

            if (rootFrame.CanGoBack)
            {
                // Show UI in title bar if opted-in and in-app backstack is not empty.
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = AppViewBackButtonVisibility.Visible;
            }
            else
            {
                // Remove the UI from the title bar if in-app back stack is empty.
                SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = AppViewBackButtonVisibility.Collapsed;
            }
        }

        private void App_BackRequested(object sender, BackRequestedEventArgs e)
        {
            Frame rootFrame = Window.Current.Content as Frame;
            if (rootFrame == null) return;

            // Navigate back if possible, and if the event has not already been handled.
            if (rootFrame.CanGoBack && e.Handled == false)
            {
                e.Handled = true;
                rootFrame.GoBack();
            }
        }

        private void AcceptButton_Click(object sender, RoutedEventArgs e)
        {
            /// Validate the user's inputs.
            if (TitleTextBox.Text == "")
            {
                popupError("The title should not be empty!");
                return;
            }
            if (DetailTextBox.Text == "")
            {
                popupError("The detail should not be empty!");
                return;
            }
            if (TodoDatePicker.Date.CompareTo(DateTime.Today) < 0)
            {
                popupError("The due date should be after current time!");
                return;
            }
            /// TODO : Save data
            this.Frame.GoBack();
        }

        /// <summary>
        /// Popup error message dialog
        /// </summary>
        /// <param name="message">Error message</param>
        private async void popupError(string message)
        {
            MessageDialog dialog = new MessageDialog(message);
            dialog.Commands.Add(new UICommand("OK"));
            await dialog.ShowAsync();
        }

        /// <summary>
        /// Reset user's inputs.
        /// </summary>
        /// <param name="sender">Sender</param>
        /// <param name="e">EventArgs</param>
        private void ClearButton_Click(object sender, RoutedEventArgs e)
        {
            TitleTextBox.Text = "";
            DetailTextBox.Text = "";
            TodoDatePicker.Date = DateTime.Today;
        }

        private async void SelectCoverButton_Click(object sender, RoutedEventArgs e)
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
                /// Load image.
                using (IRandomAccessStream fileStream = await file.OpenAsync(FileAccessMode.Read))
                {
                    BitmapImage bitmapImage = new BitmapImage();
                    await bitmapImage.SetSourceAsync(fileStream);
                    TodoCoverImage.Source = bitmapImage;
                }
            }
        }
    }
}
