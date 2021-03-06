﻿using MAD_HW4.ViewModels;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MAD_HW4 {
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ListPage : Page {
        private TodoViewModel TodoVM;

        public int SelectedItemIndex {
            get {
                return TodoListView.SelectedIndex;
            }
            set {
                if (value < TodoListView.Items.Count) {
                    if (value == -1) {
                        //(TodoListView.SelectedItem as ListViewItem).IsSelected = false;
                        TodoListView.SelectedItem = null;
                    } else {
                        TodoListView.SelectedIndex = value;
                    }
                }
            }
        }

        public ListPage() {
            InitializeComponent();
            TodoVM = TodoViewModel.getInstance();
        }

        private void TodoListView_SelectionChanged(object sender, SelectionChangedEventArgs e) {
            Frame rootFrame = Window.Current.Content as Frame;
            MainPage mainPage = rootFrame.Content as MainPage;
            mainPage.OnSelectionChanged(TodoListView.SelectedIndex);
        }
    }
}
