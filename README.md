# Strife

Making a **Discord** clone in C++ for personal project. 

## Roadmap

### Phase 1: Core Architecture & Basic Messaging
- Design and implement network protocol for client-server architecture
- Set up a basic server that can handle non-trivial amount of client connections
- Implement basic message broadcasting
- Add basic command parsing (e.g. /nick to change server nickname)

### Phase 2: User Management & Authentication
- Design user data structures & database schema
- Implement registration and login functionality
- Add session management

### Phase 3: Channels & Servers
- Design data structures for servers and channels
- Implement server/channel creation, joining, and management
- Set up message persistance in database

### Phase 4: Direct Messaging & Friends
- Implement options to add friends
- Add activity status and tracking
- Notifications for new messages

### Phase 5: Voice Chat
- Implement audio streaming between clients
- Add voice channel joining/leaving
- Add mute/deafen controls

### Phase 6: UI Enhancement & Refinement
- Make a pretty GUI
- Add file sharing support (that is **NOT** rate limited like Discord)
- Message editing and deletion

### Phase 7: Performance Optimization
- Basically just improve performance


## Thoughts
Wanted to do this to get experience and put a bigger project on my resume that is hopefully somewhat impressive. I chose C++ since I like the flexibilty of the language and I want to become better at OOP. I did debate using Rust but for now I think C++ is probably more in-demand for jobs but maybe I implement some features here in Rust, idk.