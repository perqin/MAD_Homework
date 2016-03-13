using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using System;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace MAD_HW1
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        event AnimalAskForSpeakEventHandler AnimalAskForSpeak;

        private Animal[] animals;
        private static string[] animalsName = new string[3] { "pig", "dog", "cat" };

        public MainPage()
        {
            this.InitializeComponent();

            animals = new Animal[3] { new Pig(speechTextBlock), new Dog(speechTextBlock), new Cat(speechTextBlock) };
            for (int i = 0; i < 3; ++i)
            {
                AnimalAskForSpeak += animals[i].speak;
            }
        }

        private void SpeakButton_Click(object sender, RoutedEventArgs e)
        {
            Random random = new Random();
            int i = random.Next(3);
            AnimalAskForSpeak(this, new AnimalAskForSpeakEventArgs(animalsName[i]));
        }

        private void confirmButton_Click(object sender, RoutedEventArgs e)
        {
            switch (speakerNameTextBox.Text)
            {
                case "pig":
                    AnimalAskForSpeak(this, new AnimalAskForSpeakEventArgs(animalsName[0]));
                    speakerNameTextBox.Text = "";
                    break;
                case "dog":
                    AnimalAskForSpeak(this, new AnimalAskForSpeakEventArgs(animalsName[1]));
                    speakerNameTextBox.Text = "";
                    break;
                case "cat":
                    AnimalAskForSpeak(this, new AnimalAskForSpeakEventArgs(animalsName[2]));
                    speakerNameTextBox.Text = "";
                    break;
                default:
                    break;
            }
        }
    }

    abstract class Animal
    {
        protected TextBlock textBlock;

        public Animal(TextBlock tb)
        {
            textBlock = tb;
        }

        public abstract void speak(object sender, AnimalAskForSpeakEventArgs e);

    }

    class Pig : Animal
    {
        public Pig(TextBlock tb) : base(tb)
        {
        }

        public override void speak(object sender, AnimalAskForSpeakEventArgs e)
        {
            if (e.AnimalName == "pig")
            {
                textBlock.Text += "pig: I am a pig\n";
            }
        }
    }

    class Dog : Animal
    {
        public Dog(TextBlock tb) : base(tb)
        {
        }

        public override void speak(object sender, AnimalAskForSpeakEventArgs e)
        {
            if (e.AnimalName == "dog")
            {
                textBlock.Text += "dog: I am a dog\n";
            }
        }
    }

    class Cat : Animal
    {
        public Cat(TextBlock tb) : base(tb)
        {
        }

        public override void speak(object sender, AnimalAskForSpeakEventArgs e)
        {
            if (e.AnimalName == "cat")
            {
                textBlock.Text += "cat: I am a cat\n";
            }
        }
    }

    class AnimalAskForSpeakEventArgs : EventArgs
    {
        private string animalName;

        public string AnimalName
        {
            get
            {
                return animalName;
            }
            set
            {
                animalName = value;
            }
        }

        public AnimalAskForSpeakEventArgs(string n)
        {
            AnimalName = n;
        }
    }

    delegate void AnimalAskForSpeakEventHandler(object sender, AnimalAskForSpeakEventArgs e);
}
