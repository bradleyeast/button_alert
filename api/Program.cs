using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Hosting;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

// In-memory flags
var callActive = false;
var acknowledged = false;

// POST /api/call → set call flag
app.MapPost("/api/call", () =>
{
    callActive = true;
    acknowledged = false; // reset ack
    return Results.Ok(new { callActive, acknowledged });
});

// GET /api/call → check call state
app.MapGet("/api/call", () =>
{
    return Results.Ok(new { callActive, acknowledged });
});

// DELETE /api/call → clear call
app.MapDelete("/api/call", () =>
{
    callActive = false;
    acknowledged = false;
    return Results.Ok(new { callActive, acknowledged });
});

// GET /api/acknowledge → returns "1" if acked, "0" otherwise
app.MapGet("/api/acknowledge", () =>
{
    return Results.Text(acknowledged ? "1" : "0");
});
